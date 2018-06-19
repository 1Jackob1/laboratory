#ifndef EXR1_FUNCINITIALISES_H
#define EXR1_FUNCINITIALISES_H

#include <float.h>
#include <mem.h>
#include <stdlib.h>
#include <ctype.h>
#include "SyntaxTree.h"

//-----------------------------------------------
typedef int(*func_handler)(char** s); //function type


typedef struct Stack_Tree { //stack for numbers
    t_node *data[MAX_STACK_SIZE];
    size_t size;
} t_stack_tree;

typedef struct Stack_Operations { //stack for signs
    char data[MAX_STACK_SIZE];
    size_t size;
} t_stack_operations;



int Push_Tree_Node(t_node*);
t_node* Pop_Tree_Node();

int Push_OPR(char);
char Pop_OPR();
char Peek_OPR();

void Priority_Init();
void States_Init();

int Read_Number(char**);
int Read_Spaces(char**);
int Read_Unar(char**);
int Read_Binar(char**);
int Read_Var(char**);

t_node* New_Node();
//--------------------------------

t_stack_operations Operations;
t_stack_tree Numbers;

char States[MAX_CHAR_SIZE];
char Priority[MAX_CHAR_SIZE];

//push, pop, peek for numbers stack
int Push_Tree_Node(t_node *a) {
    if(Numbers.size+1 >= MAX_STACK_SIZE)
        return ERR_SIGNS;
    Numbers.data[Numbers.size++]=a;
    return OK;
}

t_node* Pop_Tree_Node() {
    if(Numbers.size <= 0)
        return NULL;
    t_node *a = Numbers.data[--Numbers.size];
    return a;
}

//------------------------------------------------------------------------//

//push, pop, peek for operation stack
int Push_OPR(const char a) {
    if(Operations.size + 1 >= MAX_STACK_SIZE)
        return ERR_SIGNS;
    Operations.data[Operations.size++] = a;
    return OK;
}

char Peek_OPR() {
    return (Operations.size < 1) ? (char)(ERR_SIGNS) : Operations.data[Operations.size - 1];
}

char Pop_OPR() {
    if(Operations.size <= 0)
        return -1;
    return Operations.data[--Operations.size];
}
//------------------------------------------------------------------------//

t_node* New_Node() {
    t_node *tmp = malloc(sizeof(t_node));
    tmp->CHILD[0] = NULL;
    tmp->CHILD[1] = NULL;
    return tmp;
}

//0 - spaces, 1 - numbers, 2 - signs



//set default values for `Priority` and `States` arrays
void Priority_Init() {
    memset(Priority, 125, MAX_CHAR_SIZE* sizeof(char));
    Priority['$'] = 3;//unary minus
    Priority['*'] = 2;//times and div has the same priority
    Priority['/'] = 2;//--
    Priority['+'] = 1;//plus and minus has lower priority
    Priority['-'] = 1;//--
    Priority['('] = -1;//brackets have the lowest priority
    Priority[')'] = 4;//
}

void States_Init() {
    memset(States, 125, MAX_CHAR_SIZE * sizeof(char));
    for (int i = '0'; i <= '9'; ++i)
        States[i] = 1;

    for (int i = 'a'; i <= 'z'; ++i)
        States[i] = 8;
    States[' '] = 0;
    States['-'] = 2;
    States['*'] = 3;
    States['/'] = 3;
    States['+'] = 3;
    States['('] = 4;
    States[')'] = 5;
    States['\0'] = 6;
    States['\n'] = 6;
}
//------------------------------------------------------------------------//


//funcs for finite automate

int Read_Number(char **str) {
    double a = strtod(*str, str);
    t_node *tmp = New_Node();
    tmp->type = IS_NUM;
    tmp->t_data.num = a;
    Push_Tree_Node(tmp);
    return 0;
}

int Read_Spaces(char **str) {
    while(isspace(**str) && (**str != '\0'))
        ++(*str);

    return (**str == '\0') ? END_STRING : 0;
}

int Read_Unar(char **str) {
    if(**str != '-')
        return  Read_Binar(str);
    Push_OPR('$');
    ++(*str);
    return 0;
}

int Calc(char Operation) {
    t_node *F_Num = Pop_Tree_Node();

    if(F_Num == NULL)
        return ERR_SIGNS;

    t_node *tmp = New_Node();
    tmp->type = IS_OPR;

    if(Operation == '$') {
        tmp->t_data.opr = '$';
        tmp->CHILD[0] = F_Num;
        Push_Tree_Node(tmp);
        return OK;
    }

    t_node *S_Num = Pop_Tree_Node();
    if(S_Num == NULL)
        return ERR_SIGNS;

    switch (Operation) {
        case '+':
        case '-':
        case '*':
        case '/': {
           tmp->t_data.opr = Operation;
           tmp->CHILD[0] = S_Num;
           tmp->CHILD[1] = F_Num;
           Push_Tree_Node(tmp);
            break;
        }
        default:
            return ERR_SIGNS;
    }

    return OK;
}

int Read_Binar(char **str) {
    char c = **str;
    switch (c) {
        case '(': {
            Push_OPR(c);
            ++(*str);
            return OK;
        }
        case '\n': {
            return ERR_SIGNS;
        }
        case '\0': {
            return ERR_SIGNS;
        }
        case ')': {
            while((Operations.size > 0) && (Peek_OPR() != '('))
                Calc(Pop_OPR());
            if(Peek_OPR() == '(')
                Pop_OPR();
            else
                return ERR_SIGNS;
            ++(*str);
            break;
        }
        default: {
            size_t i = Operations.size;
            while((i > 0) && Priority[Operations.data[i - 1]] >= Priority[c]) {
                int tmp = Calc(Pop_OPR());
                if(tmp != OK)
                    return tmp;
                i--;
            }
            Operations.size = i;
            if(Numbers.size == 0) {
                if(c == '-')
                    return Read_Unar(str);
                return ERR_SIGNS;
            }
            Push_OPR(c);
            ++(*str);
            return OK;
        }
    }
    return OK;

}

int Read_Var(char **str) {
    t_node *tmp = New_Node();
    tmp->type = IS_VAR;
    tmp->t_data.ind = **str - 'a';
    Variables[**str - 'a'].data = 0;
    Variables[**str - 'a'].type = IS_EXIST;
    Push_Tree_Node(tmp);
    ++(*str);
    return 0;
}

int Error(char **ptr) {
    return ERROR;
}

//------------------------------------------------------------------------//
//0 - spaces, 1 - numbers, 2 - signs
int State = 0;
int PrevState = 0;

func_handler FUNCTIONS_MATRIX[3][9] = {
        {&Read_Spaces, &Read_Number, &Read_Binar, &Read_Unar,  &Read_Binar, &Read_Binar, &Read_Spaces, &Error, &Read_Var},
        {&Read_Spaces, &Error,       &Read_Binar, &Read_Binar, &Error,      &Read_Binar, &Read_Spaces, &Error, &Error   },
        {&Read_Spaces, &Read_Number, &Read_Binar, &Read_Binar, &Read_Unar,  &Read_Binar, &Read_Spaces, &Error, &Read_Var}
};

int TRANSITION_MATRIX[3][9] = {
        {0, 1,     2, 2, 2, 2, END_STRING, ERROR, 1    },
        {0, ERROR, 2, 2, 2, 2, END_STRING, ERROR, ERROR},
        {0, 1,     2, 2, 2, 2, END_STRING, ERROR, 1    }
};

void Null_State() {
    State = 0;
    PrevState = 0;
}

void Null_Stacks() {
    for (int i = 0; i < MAX_STACK_SIZE; ++i) {
        Operations.data[i] = 0;
        Numbers.data[i] = 0;
    }
    Operations.size = 0;
    Numbers.size = 0;
}

t_node* Create_Tree(char** str, double* x) {
    *x = 0;
    Priority_Init();
    States_Init();
    Null_State();
    Null_Stacks();
    for (int i = 0; i < VAR_SIZE; ++i) {
        Variables[i].data = 0;
        Variables[i].type = IS_DEFLT;
    }
    while(1){
        State = TRANSITION_MATRIX[PrevState][States[**str]];
        if(State == ERROR){
            return NULL;
        }
        if(State == END_STRING) {
            int err_code = OK;
            while(Operations.size > 0)
                err_code = Calc(Pop_OPR());
            if(err_code == OK) {
                break;
            }
            return NULL;
        }

        int tmp = FUNCTIONS_MATRIX[PrevState][States[**str]](str);
        PrevState = State;
        if(tmp == ERROR) {
            return NULL;
        } else if(tmp == END_STRING) {
            int err_code = OK;
            while(Operations.size > 0)
                err_code = Calc(Pop_OPR());
            if(err_code == OK) {
                break;
            }
            *x = DBL_MAX;
            return NULL;
        }
    }

    if(Operations.size) {
        return NULL;
    }

    return Pop_Tree_Node();
}

double Solve_Tree(t_node* node) {
    if(node == NULL)
        return 0;
    double Ztmp, Ftmp;//Zerotmp Firsttmp
    switch (node->type) {
        case IS_OPR: {
            if((Ztmp = Solve_Tree(node->CHILD[0])) == DBL_MIN)
                return DBL_MIN;
            if((Ftmp = Solve_Tree(node->CHILD[1])) == DBL_MIN)
                return DBL_MIN;
            switch (node->t_data.opr) {
                case '$':
                    return (-Ztmp);
                case '-':
                    return (Ztmp - Ftmp);
                case '+':
                    return (Ztmp + Ftmp);
                case '*':
                    return (Ztmp * Ftmp);
                case '/': {
                    if (Ftmp == 0)
                        return DBL_MIN;
                    return (Ztmp / Ftmp);
                }
                default:
                    return ERR_SIGNS;

            }
        }
        case IS_NUM:
            return node->t_data.num;
        case IS_VAR:
            return Variables[node->t_data.ind].data;
    }
}

int Set_Var(size_t ind, const double *val) {
    if((ind > 25)|| (val == NULL))
        return ERROR;
    Variables[ind].data = *val;
    Variables[ind].type = IS_SETED;
    return OK;
}
#endif //EXR1_FUNCINITIALISES_H

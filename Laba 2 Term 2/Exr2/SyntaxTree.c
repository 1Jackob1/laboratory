//
// Created by abdurashidov.aak on 21.06.2018.
//

#include "SyntaxTree.h"


#include <float.h>
#include <mem.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include "SyntaxTree.h"
/*-----------------------------------------------
 * $ - unary minus
 * @ - sin
 * # - cos
 * ! - exp
 * & - log
 * % - sqrt
 */
//-----------------------------------------------
typedef int(*func_handler)(char**, t_tree*); //function type
typedef double (*func_math_handler)(double); //math func type

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

int Read_Number(char**, t_tree*);
int Read_Spaces(char**, t_tree*);
int Read_Unar(char**, t_tree*);
int Read_Binar(char**, t_tree*);
int Read_Var(char**, t_tree*);

double Sin(double a) {
    return (a == DBL_MIN) ? DBL_MIN : sin(a);
}

double Cos(double a) {
    return (a == DBL_MIN) ? DBL_MIN : cos(a);
}

double Log(double a) {
    return (a == DBL_MIN) ? DBL_MIN : log(a);
}

double Sqrt(double a) {
    return (a == DBL_MIN) ? DBL_MIN : sqrt(a);
}

double Exp(double a) {
    return (a == DBL_MIN) ? DBL_MIN : exp(a);
}

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
    Priority['@'] = 3;
    Priority['#'] = 3;
    Priority['%'] = 3;
    Priority['&'] = 3;
    Priority['!'] = 3;
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
    States['!'] = 9;
    States['@'] = 9;
    States['#'] = 9;
    States['%'] = 9;
    States['&'] = 9;
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

int Read_Number(char **str, t_tree *src) {
    double a = strtod(*str, str);
    t_node *tmp = New_Node();
    tmp->type = IS_NUM;
    tmp->t_data.num = a;
    Push_Tree_Node(tmp);
    return 0;
}

int Read_Spaces(char **str, t_tree *src) {
    while(isspace(**str) && (**str != '\0'))
        ++(*str);

    return (**str == '\0') ? END_STRING : 0;
}

int Read_Unar(char **str, t_tree *src) {
    if(**str != '-')
        return  Read_Binar(str, src);
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
        return Push_Tree_Node(tmp);
    }

    switch (Operation) {
        case '@':
            tmp->t_data.ind = 0;
            break;
        case '#':
            tmp->t_data.ind = 1;
            break;
        case '!':
            tmp->t_data.ind = 2;
            break;
        case '%':
            tmp->t_data.ind = 3;
            break;
        case '&':
            tmp->t_data.ind = 4;
            break;
        default:
            break;
    }

    if((Operation == '!') || (Operation == '@') || (Operation == '#') || (Operation == '%') || (Operation == '&')) {
        tmp->type = IS_FUN;
        tmp->CHILD[0] = F_Num;
        return Push_Tree_Node(tmp);
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

int Read_Binar(char **str, t_tree *src) {
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
                    return Read_Unar(str, src);
                return ERR_SIGNS;
            }
            Push_OPR(c);
            ++(*str);
            return OK;
        }
    }
    return OK;

}

int Read_Var(char **str, t_tree *src) {
    if((strlen(*str) > 4) && (**str == 's' || **str == 'c' || **str == 'e' || **str == 'l')) {
        if((**str == 's') && (*((*str) + 1) == 'i') && (*((*str) + 2) == 'n')) {
            (*str) += 3;
            return Push_OPR('@');
        }
        if((**str == 'c') && (*((*str) + 1) == 'o') && (*((*str) + 2) == 's')) {
            (*str) += 3;
            return Push_OPR('#');;
        }
        if((**str == 'e') && (*((*str) + 1) == 'x') && (*((*str) + 2) == 'p')) {
            (*str) += 3;
            return Push_OPR('!');
        }
        if((**str == 'l') && (*((*str) + 1) == 'o') && (*((*str) + 2) == 'g')) {
            (*str) += 3;
            return Push_OPR('&');
        }
        if((**str == 's') && (*((*str) + 1) == 'q') && (*((*str) + 2) == 'r') && (*((*str) + 3) == 't')) {
            (*str) += 4;
            return Push_OPR('%');
        }
    }
    t_node *tmp = New_Node();
    tmp->type = IS_VAR;
    tmp->t_data.ind = **str - 'a';
    src->Vars[**str - 'a'].data = 0;
    src->Vars[**str - 'a'].type = IS_EXIST;
    Push_Tree_Node(tmp);
    ++(*str);
    return 0;
}

int Error(char **ptr, t_tree *src) {
    return ERROR;
}

//------------------------------------------------------------------------//
//0 - spaces, 1 - numbers, 2 - signs
int State = 0;
int PrevState = 0;

func_handler FUNCTIONS_MATRIX[3][10] = {
        {&Read_Spaces, &Read_Number, &Read_Binar, &Read_Unar,  &Read_Binar, &Read_Binar, &Read_Spaces, &Error, &Read_Var, &Read_Var},
        {&Read_Spaces, &Error,       &Read_Binar, &Read_Binar, &Read_Binar,      &Read_Binar, &Read_Spaces, &Error, &Error   , &Read_Var},
        {&Read_Spaces, &Read_Number, &Read_Binar, &Read_Binar, &Read_Unar,  &Read_Binar, &Read_Spaces, &Error, &Read_Var, &Read_Var}
};

int TRANSITION_MATRIX[3][10] = {
        {0, 1,     2, 2, 2, 2, END_STRING, ERROR, 1,     2},
        {0, ERROR, 2, 2, 2, 2, END_STRING, ERROR, ERROR, 2},
        {0, 1,     2, 2, 2, 2, END_STRING, ERROR, 1,     2}
};

const func_math_handler Math_Functions[5] = { &Sin, &Cos, &Exp, &Sqrt, &Log };

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

t_tree* Create_Tree(char** str, double* x) {
    *x = 0;
    Priority_Init();
    States_Init();
    Null_State();
    Null_Stacks();
    t_tree *tree_root = malloc(sizeof(t_tree));
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

        int tmp = FUNCTIONS_MATRIX[PrevState][States[**str]](str, tree_root);
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

    tree_root->root = Pop_Tree_Node();
    for (int i = 0; i < VAR_SIZE; ++i) {
        tree_root->Vars[i].data = 0;
    }
    return tree_root;
}



double Solve_Tree_Nodes(t_node* node, t_var *Variables) {
    if(node == NULL)
        return 0;
    double Ztmp, Ftmp;//Zerotmp Firsttmp
    switch (node->type) {
        case IS_OPR: {
            if((Ztmp = Solve_Tree_Nodes(node->CHILD[0], Variables)) == DBL_MIN)
                return DBL_MIN;
            if((Ftmp = Solve_Tree_Nodes(node->CHILD[1], Variables)) == DBL_MIN)
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
                    return DBL_MIN;

            }
        }
        case IS_NUM:
            return node->t_data.num;
        case IS_VAR:
            return Variables[node->t_data.ind].data;
        case IS_FUN:
            Ztmp = Solve_Tree_Nodes(node->CHILD[0], Variables);
            if((node->t_data.ind == 4) && (Ztmp <= 0))
                return DBL_MIN;
            return (Ztmp == DBL_MIN) ? DBL_MIN : Math_Functions[node->t_data.ind](Ztmp);
    }
}

double Solve_Tree(t_tree* root) {
    return Solve_Tree_Nodes(root->root, root->Vars);
}

int Set_Var(t_tree *root, size_t ind, const double *val) {
    if((ind > 25)|| (val == NULL))
        return ERROR;
    root->Vars[ind].data = *val;
    root->Vars[ind].type = IS_SETED;
    return OK;
}


t_node* Copy_Tree_Nodes(t_node *src) {
   t_node *tmp = malloc(sizeof(t_node));
   if(tmp == NULL)
       return NULL;
   *tmp = *src;
   if(src->CHILD[0] == NULL)
       return tmp;
   tmp->CHILD[0] = Copy_Tree_Nodes(src->CHILD[0]);
   if(src->CHILD[1] == NULL)
       return tmp;
   tmp->CHILD[1] = Copy_Tree_Nodes(src->CHILD[1]);
    return tmp;
}

t_tree* Copy_Tree(t_tree *src) {
    t_tree *new_root = malloc(sizeof(t_tree));
    new_root->root = Copy_Tree_Nodes(src->root);
    for (int i = 0; i < VAR_SIZE; ++i) {
        new_root->Vars[i] = src->Vars[i] ;
    }
    return new_root;
}

void Delete_Tree_Nodes(t_node *node) {
    if (node == NULL)
        return;
    Delete_Tree_Nodes(node->CHILD[0]);
    Delete_Tree_Nodes(node->CHILD[1]);
    free(node);
}

int Delete_Tree(t_tree **src) {
    Delete_Tree_Nodes((*src)->root);
    free(*src);
    *src = NULL;
    return OK;
}

void Print_Tree(t_node *src) {
    if (src == NULL)
        return;
    switch (src->type){
        case IS_NUM:
            printf(" %lf ", src->t_data.num);
            return;
        case IS_VAR:
            printf(" %c ", src->t_data.ind + 'a');
            return;
        case IS_OPR:
            if (src->CHILD[1] == NULL){
                printf(" %c(", src->t_data.opr);
                Print_Tree(src->CHILD[0]);
                printf(" ) ");
                return;
            }
            printf("( ");
            Print_Tree(src->CHILD[0]);
            printf("%c ", src->t_data.opr );
            Print_Tree(src->CHILD[1]);
            printf(" ) ");
            return;
        case IS_FUN:
            switch (src->t_data.ind) {
                case 0:
                    printf("sin(");
                    break;
                case 1:
                    printf("cos(");
                    break;
                case 2:
                    printf("exp(");
                    break;
                case 3:
                    printf("sqrt(");
                    break;
                case 4:
                    printf("log(");
            }
            Print_Tree(src->CHILD[0]);
            printf(" ) ");
            return;
    }
}

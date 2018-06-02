//
// Created by abdurashidov.aak on 02.06.2018.
//

#ifndef EXR1_FUNCINITIALISES_H
#define EXR1_FUNCINITIALISES_H

#include <float.h>
#include <mem.h>
#include "defandtypes.h"

t_stack_operations Operations;
t_stack_number Numbers;

char States[MAX_CHAR_SIZE];
char Priority[MAX_CHAR_SIZE];





//push, pop, peek for numbers stack
int Push_Num(double a){
    if(Numbers.size+1>=MAX_STACK_SIZE)
        return ERR_SIGNS;
    Numbers.data[Numbers.size++]=a;
    return OK;
}

double Peek_Num(){
    return (Numbers.size<1) ? DBL_MAX : Numbers.data[Numbers.size-1];
}

double Pop_Num(){
    if(Numbers.size<=0)
        return DBL_MIN;
    double a=Numbers.data[--Numbers.size];
    return a;
}
//------------------------------------------------------------------------//

//push, pop, peek for operation stack
int Push_OPR(const char a){
    if(Operations.size+1>=MAX_STACK_SIZE)
        return ERR_SIGNS;
    Operations.data[Operations.size++]=a;
    return OK;
}

char Peek_OPR(){
    return (Operations.size<1) ? (char)(ERR_SIGNS) : Operations.data[Operations.size-1];
}

char Pop_OPR(){
    if(Operations.size<=0)
        return -1;
    return Operations.data[--Operations.size];
}
//------------------------------------------------------------------------//

//0 - spaces, 1 - numbers, 2 - signs



//set default values for `Priority` and `States` arrays
void Priority_Init(){
    memset(Priority, 125, MAX_CHAR_SIZE* sizeof(char));
    Priority['$']=3;//unary minus
    Priority['*']=2;//times and div has the same priority
    Priority['/']=2;//--
    Priority['+']=1;//plus and minus has lower priority
    Priority['-']=1;//--
    Priority['(']=-1;//brackets have the lowest priority
    Priority[')']=4;//
}

void States_Init(){
    memset(States, 125, MAX_CHAR_SIZE * sizeof(char));
    for (int i = '0'; i <= '9'; ++i) {
        States[i]=1;
    }
    States[' ']=0;
    States['-']=2;
    States['*']=3;
    States['/']=3;
    States['+']=3;
    States['(']=4;
    States[')']=5;
    States['\0']=6;
    States['\n']=6;
}
//------------------------------------------------------------------------//


//funcs for finite automate

int Read_Number(char** str){
    double a=strtod(*str, str);
    Push_Num(a);
    return 0;
}

int Read_Spaces(char** str){
    while(isspace(**str) && (**str != '\0'))
        ++(*str);

    return (**str=='\0') ? END_STRING : 0;
}

int Read_Unar(char** str){
    if(**str != '-')
        return  Read_Binar(str);
    Push_OPR('$');
    ++(*str);
    return 0;
}

int Calc(char Operation){
    double F_Num=Pop_Num();

    if(F_Num==DBL_MIN)
        return ERR_SIGNS;

    if(Operation=='$'){
        F_Num*=(-1);
        Push_Num(F_Num);
        return OK;
    }

    double S_Num=Pop_Num();
    if(S_Num == DBL_MIN)
        return ERR_SIGNS;

    switch (Operation){
        case '+':{
            Push_Num(F_Num+S_Num);
            break;
        }
        case '-':{
            Push_Num(S_Num-F_Num);
            break;
        }
        case '*':{
            Push_Num(F_Num*S_Num);
            break;
        }
        case '/':{
            if(F_Num==0)
                return ERR_ZERO;
            Push_Num(S_Num/F_Num);
            break;
        }
        default:
            return ERR_SIGNS;
    }

    return OK;
}

int Read_Binar(char** str){
    char c=**str;
    switch (c){
        case '(':{
            Push_OPR(c);
            ++(*str);
            return OK;
        }
        case '\n':{
            return ERR_SIGNS;
        }
        case '\0':{
            return ERR_SIGNS;
        }
        case ')':{
            while((Operations.size>0) && (Peek_OPR()!='('))
                Calc(Pop_OPR());
            if(Peek_OPR()=='(')
                Pop_OPR();
            else
                return ERR_SIGNS;
            ++(*str);
            break;
        }
        default:{
            size_t i=Operations.size;
            while((i>0) && Priority[Operations.data[i-1]]>=Priority[c]){
                int tmp=Calc(Pop_OPR());
                if(tmp != OK)
                    return tmp;
                i--;
            }
            Operations.size=i;
            if(Numbers.size==0){
                return Read_Unar(str);
            }
            Push_OPR(c);
            ++(*str);
            return OK;
        }
    }
    return OK;

}

int Error(char** ptr){
    return ERROR;
}
//------------------------------------------------------------------------//

int State=0;
int PrevState=0;
f FUNCTIONS_MATRIX[3][8]={
        {&Read_Spaces,&Read_Number,&Read_Binar,&Read_Unar,&Read_Binar,&Read_Binar,&Read_Spaces,&Error},
        {&Read_Spaces,&Error,&Read_Binar,&Read_Binar,&Error,&Read_Binar,&Read_Spaces,&Error},
        {&Read_Spaces,&Read_Number,&Read_Binar,&Read_Binar,&Read_Unar,&Read_Binar,&Read_Spaces,&Error}
};

int TRANSITION_MATRIX[3][8]={
        {0, 1, 2, 2, 2, 2, END_STRING, ERROR},
        {0, ERROR, 2, 2, 2, 2, END_STRING, ERROR},
        {0, 1, 2, 2, 2, 2, END_STRING, ERROR}
};


int Solve(char** str, double* x){
    *x=0;
    Priority_Init();
    States_Init();
    int code=0;
    while(1){
        State=TRANSITION_MATRIX[PrevState][States[**str]];
        if(State==ERROR){
//            *x=DBL_MIN;
            return ERROR;
        }
        if(State==END_STRING){
            int err_code=OK;
            while(Operations.size>0)
                err_code=Calc(Pop_OPR());
            if(err_code == OK){
                code = OK;
                break;
            }
//            *x=DBL_MIN;
            return err_code;
        }

        int tmp = FUNCTIONS_MATRIX[PrevState][States[**str]](str);
        PrevState = State;
        if(tmp==ERROR){
            return tmp;
        } else if(tmp==END_STRING){
            int err_code=OK;
            while(Operations.size>0)
                err_code=Calc(Pop_OPR());
            if(err_code == OK){
                code = OK;
                break;
            }
            *x=DBL_MIN;
            return err_code;
        }
    }

    if(Operations.size) {
        *x=DBL_MAX;
        return ERR_BRACKETS;
    }

    *x=Peek_Num();
    if(*x == DBL_MAX)
        return ERR_SIGNS;
//    printf("%lf\n", *x);
    return code;
}
#endif //EXR1_FUNCINITIALISES_H

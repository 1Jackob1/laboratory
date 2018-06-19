//
// Created by abdurashidov.aak on 02.06.2018.
//

#ifndef EXR1_DEFINES_H
#define EXR1_DEFINES_H

#define MAX_STACK_SIZE 100
#define MAX_CHAR_SIZE 127

#define OK 0
#define ERR_ZERO 1
#define ERR_BRACKETS 2
#define ERR_SIGNS 3
#define END_STRING 6
#define ERROR 3

typedef int(*f)(char** s); //function type

typedef struct Stack_Number{ //stack for numbers
    double data[MAX_STACK_SIZE];
    size_t size;
}t_stack_number;

typedef struct Stack_Operations{ //stack for signs
    char data[MAX_STACK_SIZE];
    size_t size;
}t_stack_operations;

int Push_Num(double);
double Pop_Num();
double Peek_Num();

int Push_OPR(char);
char Pop_OPR();
char Peek_OPR();

void Priority_Init();
void States_Init();

int Read_Number(char**);
int Read_Spaces(char**);
int Read_Unar(char**);
int Read_Binar(char**);

int Solve(char**, double*);

#endif //EXR1_DEFINES_H

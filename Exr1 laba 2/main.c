#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include <float.h>
#include <ctype.h>

#include "cmake-build-debug/defandtypes.h"
#include "cmake-build-debug/FuncInitialises.h"




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



int main() {
    int i=0, BlockSize=15, Step=50;
    char *s=(char*)malloc(BlockSize * sizeof(char));

    double x;
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");
    while((s[i]=(char)fgetc(in)) != EOF){
        if(++i>=BlockSize) {
            BlockSize+=Step;
            s=(char*)realloc(s, sizeof(char) * BlockSize);
        }
    }
    s[i]='\0';
    int code=Solve(&s, &x);
    if(code == OK)
        fprintf(out ,"done with code: %d, ans: %lf",code, x);
    else
        fprintf(out, "done with code: %d", code);
    free(s);
    fclose(in);
    fclose(out);
    return 0;
}
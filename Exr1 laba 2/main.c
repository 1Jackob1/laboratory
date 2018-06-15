#include <stdio.h>
#include <malloc.h>


#include "cmake-build-debug/defandtypes.h"

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
    char *saveS = s;
    int code=Solve(&s, &x);
    if(code == OK)
        fprintf(out ,"done with code: %d, ans: %lf",code, x);
    else
        fprintf(out, "done with code: %d", code);
    s=saveS;
    free(s);
    fclose(in);
    fclose(out);
    return 0;
}

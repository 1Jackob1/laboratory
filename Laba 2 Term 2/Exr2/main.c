#include <stdio.h>
#include <malloc.h>
#include <mem.h>


#include "SyntaxTree.h"

int main() {
    int i = 0, BlockSize = 70;
    char *s = (char*)malloc(BlockSize * sizeof(char));
    double x;
    FILE *in = fopen("input.txt", "r");
    FILE *out = fopen("output.txt", "w");
    char *s0 = s;
    while((s[i] = (char)fgetc(in)) != EOF){
        if(++i >= BlockSize) {
            BlockSize += STEP;
            s = (char*)realloc(s, sizeof(char) * BlockSize);
        }
    }
    s[i] = '\0';
    t_node* root = Create_Tree(&s, &x);
    i = 0;
    while(i<VAR_SIZE) {
        if(Variables[i].type != IS_EXIST) {
            i++;
            continue;
        }

        printf("Please set variable \"%c\" : ", i + 'a');
        double tmp = 0;
        scanf("%lf", &tmp);
        if(Set_Var(i, &tmp) != OK){
            printf("\nCan't set value, pls try again!\n");
            continue;
        }
        i++;
    }
    double ans = Solve_Tree(root);
    printf("%lf", ans);
    free(s0);
    fclose(in);
    fclose(out);
    return 0;
}
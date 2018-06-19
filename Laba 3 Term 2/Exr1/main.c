#include <stdio.h>
#include <malloc.h>

#include "Hash_Table_With_List.h"

t_cell Table[MAXSIZE];

int main() {
    FILE* in=fopen("input.txt", "r");
    FILE* out=fopen("output.txt", "w");
    char* s=malloc(sizeof(char) * STRING_READING_STEP);
    while(GetWord(s, in)){
//        printf("maybe here: %s \n", s);
        int code = Add2Table(s, Table);
        if(code == OK)
            continue;
        fprintf(out,"Sorry, there are no memory left!");
        break;
    }
//    printf("cycle ends;");
    PrintTable(Table, out);
    fclose(in);
    fclose(out);
    printf("\ndone\n");
    return 0;
}
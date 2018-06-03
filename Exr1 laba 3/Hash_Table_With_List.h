//
// Created by abdurashidov.aak on 03.06.2018.
//

#ifndef LABA_3_TERM_2_HASH_TABLE_WITH_LIST_H
#define LABA_3_TERM_2_HASH_TABLE_WITH_LIST_H
#endif //LABA_3_TERM_2_HASH_TABLE_WITH_LIST_H

#define MAXSIZE 80000
#define BUFFSIZE 4096
#define PRIMENUMBER 59
#define STRING_READING_STEP 100
#define NO_MEMORY (-1)
#define OK 0

typedef struct t_cell{
    struct t_cell* next;
    char* word;
    int count;
}t_cell;

t_cell* NewNode();
int AddNode(t_cell*, char*);

int CalcHash(char*);

int GetChar(FILE*);
int GetWord(char*, FILE*);

int Add2Table(char*, t_cell*);

void PrintTable(t_cell*, FILE*);
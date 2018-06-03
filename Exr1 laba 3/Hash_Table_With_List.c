//
// Created by abdurashidov.aak on 03.06.2018.
//
#include <ctype.h>
#include <mem.h>
#include <stdlib.h>
#include <stdio.h>
#include "Hash_Table_With_List.h"



int cmp(const void* lhs, const void* rhs){
    return strcmp(((t_cell*)(lhs))->word, ((t_cell*)(rhs))->word);
}

int CalcHash(char* s){
    unsigned long long hash=0;
    int hashPower=1;
    for (int i = 0; i < strlen(s); ++i) {
        if(s[i]=='\0') break;
        hash+=s[i]*hashPower;
        hashPower*=PRIMENUMBER;
    }
    return (int)(hash%MAXSIZE);
}

int pos=0, len=0;//only for GetChar() function!
char BUFF[BUFFSIZE];//only for GetChar() function!

int GetChar(FILE *f) {
    if(pos == len) {
        len = fread(BUFF, 1, BUFFSIZE, f);
        pos = 0;
        if (len == 0) return EOF;
    }
    return tolower(BUFF[pos++]);
}

int GetWord(char *s, FILE* f){
    int _len=0;
    int step=STRING_READING_STEP;
    char c;
    while( 1 ){
        if(_len+1>step){
            step+=STRING_READING_STEP;
            s=(char*)realloc(s, sizeof(char) * step);
        }
        if((c=(char)GetChar(f)) == EOF){
            if(_len == 0)
                _len = -1;
            else
                s[_len]='\0';
            break;
        }
        if(c==' ' || (!isalpha(c) && !isdigit(c))) {
            if(_len == 0)
                continue;
            s[_len] = '\0';
            break;
        }
        else
            s[_len++]=c;
    }
    s[_len]='\0';
    return (_len > 0) ? 1 : 0;
}

t_cell* NewNode(){
    t_cell* new=(t_cell*)malloc(sizeof(t_cell));
    if(new == NULL)
        return NULL;
    new->next=NULL;
    new->count=0;
    return new;
}

int AddNode(t_cell* src, char* str){
   src->next=NewNode();
   if(src->next == NULL)
       return NO_MEMORY;
   src->next->word=(char*)malloc(sizeof(char) * (strlen(str) < STRING_READING_STEP) ? STRING_READING_STEP : strlen(str));
   strcpy(src->next->word, str);
   src->next->count=1;
    return OK;
}

int Add2Table(char* s, t_cell* TB){
    int hash = CalcHash(s);
//    printf("\nbefore first insert\n");
    if(TB[hash].count == 0) {
        TB[hash].word=(char *)malloc(sizeof(char) * (strlen(s) < STRING_READING_STEP) ? STRING_READING_STEP : strlen(s));
        strcpy(TB[hash].word, s);
        TB[hash].count++;
        return OK;
    }
//    printf("\nafter first before second\n");
    if(strcmp(TB[hash].word, s) == 0){
        TB[hash].count++;
        return OK;
    }
//    printf("\n after second before NULL cheking");
    if(TB[hash].next == NULL)
        return AddNode(&TB[hash], s);
//    printf("\n after NULL checking before linked list creation\n");
    t_cell* tmp=TB[hash].next;
    while(tmp != NULL){
        if(strcmp(tmp->word, s) == 0){
            tmp->count++;
            return OK;
        }
        if(tmp->next == NULL)
            break;
        tmp=tmp->next;
    }

    return AddNode(tmp, s);

}

void PrintTable(t_cell* TB, FILE* f){
    for (int i = 0; i < MAXSIZE; ++i) {
        if(TB[i].count != 0){
            fprintf(f,"word: %s, repeats: %d, times\n", TB[i].word, TB[i].count);
            if(TB[i].next == NULL)
                continue;
            t_cell* tmp = TB[i].next;
            do{
                fprintf(f,"word: %s, repeats: %d, times\n", tmp->word, tmp->count);
                tmp=tmp->next;
            }while(tmp != NULL);
        }
    }
}
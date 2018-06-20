//
// Created by abdurashidov.aak on 14.06.2018.
//

//
// Created by Danila on 14.06.2018.
//
#include <stdlib.h>
#include <string.h>


#include "Cstring.h"

int STRING_new(STRING_t_hand *hand, const char *str){
    if((STRING_off(hand) == STRING_ERR_DATA) || (str == NULL)){
        return STRING_ERR_DATA;
    }
    *hand = malloc(sizeof(STRING_t_head));
    if(*hand == NULL){
        return STRING_ERR_NULL;
    }
    (*hand)->word = malloc(strlen(str) + 1);
    if((*hand)->word == NULL){
        free(*hand);
        *hand = NULL;
        return STRING_ERR_NULL;
    }
    (*hand)->count=1;
    strcpy((*hand)->word, str); // проверить
    return  STRING_ERR_NORM;
}

int STRING_off(STRING_t_hand *hand){
    if(hand == NULL){
        return STRING_ERR_DATA;
    }
    if((*hand) == NULL){
        return STRING_ERR_NORM;
    }
    ((*hand)->count)--;
    if((*hand)->count == 0){
        free((*hand)->word);
        free(*hand);
    }
    *hand = NULL;
    return STRING_ERR_NORM;
}

int STRING_set(STRING_t_hand *hand, STRING_t_hand src){
    if(hand == NULL){
        return STRING_ERR_DATA;
    }
    if((*hand) == src){
        return STRING_ERR_NORM;
    }
    STRING_off(hand);
    *hand = src;
    (src->count)++;
    return STRING_ERR_NORM;
}

int STRING_cpy(STRING_t_hand *hand, STRING_t_hand src){
    if((src == NULL) || (hand == NULL)){
        return STRING_ERR_DATA;
    }
    if(((*hand) == src) && (src->count == 1)) return STRING_ERR_NORM;
    int error = STRING_new(hand, src->word);
    return error;
}

int STRING_sub(STRING_t_hand *hand, STRING_t_hand src, size_t start, size_t size){
    if((src == NULL) || (size == 0) || (strlen(src->word) < (size + start))){
        return STRING_ERR_DATA;
    }
    char *word = malloc(size + 1);
    if(word == NULL){
        STRING_off(hand);
        return STRING_ERR_NULL;
    }
    strncpy(word, src->word + start, size);
    STRING_off(hand);
    *hand = malloc(sizeof(STRING_t_hand));
    if(*hand == NULL){
        free(word);
        return STRING_ERR_NULL;
    }
    (*hand)->word=word;
    (*hand)->word[size]='\0';
    (*hand)->count = 1;
    return STRING_ERR_NORM;
}

int STRING_cat(STRING_t_hand *hand, STRING_t_hand lhs, STRING_t_hand rhs){
    if((hand == NULL) || (lhs == NULL) || (rhs == NULL)){
        return STRING_ERR_DATA;
    }
    STRING_t_head *temp = malloc(sizeof(STRING_t_head));
    if(temp == NULL){
        STRING_off(hand);
        return STRING_ERR_NULL;
    }
    size_t len = strlen(lhs->word);
    (temp)->word = malloc(len + strlen(rhs->word) + 1);
    if((temp)->word == NULL){
        STRING_off(hand);
        return STRING_ERR_NULL;
    }
    strcpy((temp)->word, lhs->word);
    strcpy((temp)->word + len, rhs->word);
    (temp)->count = 1;
    STRING_off(hand);
    *hand = temp;
    return STRING_ERR_NORM;
}

int STRING_add(STRING_t_hand *hand, STRING_t_hand lhs, char end){
    if(hand == NULL || lhs == NULL)
        return STRING_ERR_DATA;
    if(*hand == lhs){
        size_t len = strlen((lhs)->word);
        (lhs)->word = realloc((*hand)->word, len + 2);
        if((lhs)->word == NULL){
            STRING_off(hand);
            return STRING_ERR_NULL;
        }
        (lhs)->word[len]=end;
        (lhs)->word[len+1]='\0';
        *hand = lhs;
        return STRING_ERR_NORM;
    }
    int err = STRING_off(hand);
    if(err != STRING_ERR_NORM)
        return err;
    STRING_t_head* temp = malloc(sizeof(STRING_t_head));
    if(temp == NULL)
        return STRING_ERR_NULL;
    size_t len = strlen(lhs->word);
    temp->word = malloc(len+2);
    if(temp->word == NULL)
        return STRING_ERR_NULL;
    strcpy(temp->word, lhs->word);
    strcpy(temp->word + len, &end);
    temp->word[len+1]='\0';
    *hand = temp;
    return STRING_ERR_NORM;
}

int STRING_cmp(const STRING_t_hand lhs, const STRING_t_hand rhs){
    if(lhs == NULL || rhs == NULL)
        return STRING_ERR_DATA;
    int pos = strcmp(lhs->word, rhs->word);
    if(pos == 0)
        return pos;
    if(pos>0)
        return 1;
    if(pos<0)
        return (-1);
}

int STRING_get(const STRING_t_hand hand, size_t ind){
    if(hand == NULL || strlen(hand->word) < ind)
        return STRING_ERR_DATA;
    return hand->word[ind];

}

int STRING_len(const STRING_t_hand hand){
    if(hand == NULL)
        return STRING_ERR_DATA;
    return strlen(hand->word);
}

void STRING_print(const STRING_t_hand hand){
    if(hand == NULL)
        return;
    printf("%s\n", hand->word);
}


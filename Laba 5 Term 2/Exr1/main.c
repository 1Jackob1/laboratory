#include "Cstring.h"

STRING_t_hand test, test2 = NULL, test3, test4;
int main(){

    int err = STRING_new(&test, "test");
    printf("created new string with code: %d \n", err);
    STRING_print(test);
    err = STRING_set(&test2, test);
    printf("STRING_set(&test2, test) done with code: %d\n", err);
    STRING_print(test2);
    STRING_new(&test3, "vahob");
    STRING_new(&test4, "molodec");
    STRING_print(test3);
    err = STRING_cpy(&test, test3);
    printf("STRING_cpy(&test, test3) done with code: %d\n print test-> ", err);
    STRING_print(test);

    err = STRING_sub(&test, test4, 4, 3);

    printf("STRING_sub(&test, test4, 3, 3) done with code: %d\n print test-> ", err);
    STRING_print(test);

    err = STRING_cat(&test3, test,test4);
    printf("STRING_cat(&test3, test,test4) done with code: %d\n print test3-> ", err);
    STRING_print(test3);

    err = STRING_add(&test3, test, 'z');
    printf("STRING_add(&test3, test, 'z') done with code: %d\n print test3-> ", err);
    STRING_print(test3);

    err = STRING_cmp(test3, test);
    printf("STRING_cmp(test, test3) done with code: %d\n test ->  ", err);
    STRING_print(test3);
    printf("test3 -> ");
    STRING_print(test);

    err = STRING_get(test, 2);
    printf("STRING_get(test, 2) %c \n", err);
    err = STRING_len(test);
    printf("STRING_len(test) %d", err);

}
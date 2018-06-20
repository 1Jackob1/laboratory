#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Объявление используемых типов данных:
 */

/* Предварительное объявление структуры */

/* Тип дескриптора строки */

/* Признак открепленного дескриптора (используется при инициализации) */
#define STRING_NULL (NULL)

/* Коды ошибок */
#define STRING_ERR_NORM (0)	//Ошибки отсутствуют;
#define STRING_ERR_NULL (-1)	//Ошибка выделения памяти;
#define STRING_ERR_DATA (-2)	//Некорректные входные параметры;

/**
 * Функции для работы со строками:
 */

/* Генерирует новую строку на основе массива символов и связывает ее с заданным указателем */
/* Возвращает код ошибки */
extern int STRING_new(STRING_t_hand *hand, const char *buff);

/* Открепляет указатель от связанной с ним строки */
/* Возвращает код ошибки */
extern int STRING_off(STRING_t_hand *hand);

/* Присваивает указателю ссылку на строку, связанную с другим указателем */
/* Возвращает код ошибки */
extern int STRING_set(STRING_t_hand *hand, STRING_t_hand src);

/* Копирует строку в новую область памяти и связывает ее с заданным указателем */
/* Возвращает код ошибки */
extern int STRING_cpy(STRING_t_hand *hand, STRING_t_hand src);

/* Копирует подстроку в новую область памяти и связывает ее с заданным указателем */
/* Возвращает код ошибки */
extern int STRING_sub(STRING_t_hand *hand, STRING_t_hand src, size_t start, size_t size);

/* Выполняет конкатенацию двух строк и связывает результат с заданным указателем */
/* Возвращает код ошибки */
extern int STRING_cat(STRING_t_hand *hand, STRING_t_hand lhs, STRING_t_hand rhs);

/* Генерирует новую строку, путем добавления символа в конец существующей строки,
   и связывает результат с заданным указателем */
/* Возвращает код ошибки */
extern int STRING_add(STRING_t_hand *hand, STRING_t_hand lhs, char end);

/* Выполняет сравнение двух строк в лексикографическом порядке */
/* Возвращает:
   0 -- если строки совпадают,
  +1 -- если первая строка больше второй,
  -1 -- если меньше */
extern int STRING_cmp(const STRING_t_hand lhs, const STRING_t_hand rhs);

/* Возвращает значение символа по индексу */
extern int STRING_get(const STRING_t_hand hand, size_t ind);

/* Возвращает размер строки */
extern int STRING_len(const STRING_t_hand hand);

extern void STRING_print(const STRING_t_hand hand);

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
    getchar();
}

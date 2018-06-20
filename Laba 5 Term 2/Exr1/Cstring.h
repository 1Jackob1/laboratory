/**
 * Текущий заголовочный файл содержит интерфейсы функций, предназначенных для работы со строками
 * посредством "умных указателей" (SmartPointer) с подсчетом ссылок.
 */

#ifndef __INCLUDE_STRING_H
#define __INCLUDE_STRING_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * Объявление используемых типов данных:
 */

/* Предварительное объявление структуры */
typedef struct STRING_t_head {
    char* word;
    size_t count;


}STRING_t_head;

/* Тип дескриптора строки */
typedef STRING_t_head *STRING_t_hand;

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

#endif //__INCLUDE_STRING_H


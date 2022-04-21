#ifndef COLUMN_H
#define COLUMN_H
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_STR_LEN 256

// все существующие типы, которые будут принимать значения колонок
typedef enum
{
    INT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
    STRING_TYPE
} TYPE;

// структура колонки
typedef struct _column
{
    void** values;
    TYPE type;
    int size;
    int len;
} column;

#endif

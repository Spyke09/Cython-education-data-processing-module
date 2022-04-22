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

void set_str(column* a, vector_char_t* value, int ind);
void set_char(column* a, char value, int ind, int jnd);
void set_int(column* a, int value, int ind);
void set_double(column* a, double value, int ind);

void set_bool(column* a, int value, int ind);
vector_char_t* get_str(column* a, int ind);
int get_int(column* a, int ind);
int get_int(column* a, int ind);
int get_bool(column* a, int ind);

char* vec_to_str(vector_char_t* v);
void str_to_vec(vector_char_t* t, char* c);

void str_to_vec(vector_char_t* t, char* c);
column* create_column(TYPE a, int n);
void print_column(column* a);
column* get_column_str(char** str, int len);
void clear_column(column** a);
column* get_typed_column_from_str(column** an, TYPE type);
column* get_typed_column_from_int(column** an, TYPE type);
column* get_typed_column_from_double(column** an, TYPE type);
column* get_typed_column(column** an, TYPE type);
void type_column(column** an, TYPE type);

#endif

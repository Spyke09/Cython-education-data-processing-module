#ifndef TABLE_H
#define TABLE_H

#include "column.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Python.h>

#define LEN 1000

typedef struct
{
    PyObject_HEAD
    column* col;
    //vector_char_t* name;
    char* name;
} py_column;

typedef struct
{
    py_column** columns;
    int len;
    char** c_names;
} table;

table* table_from_csv(int rows, int columns, char* filename, char* delim);

#endif TABLE_H

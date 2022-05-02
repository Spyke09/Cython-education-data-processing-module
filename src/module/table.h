#ifndef TABLE_H
#define TABLE_H

#include "column.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vector.h"

#define LEN 1000

typedef struct
{
    column** columns;
    int len;
} table;

table* table_from_csv(char* filename, char delim);
void print_table(table* table, int n);
void print_column_types(table* table);
table* copy_table(table* t);

#endif TABLE_H

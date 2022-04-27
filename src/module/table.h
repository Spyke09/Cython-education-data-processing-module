#include "column.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN 1000

typedef struct _table
{
    column** columns;
    int len;
    char** c_names;
} table;

table* read_csv(int rows, int columns, char* filename, char* delim);

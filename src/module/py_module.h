#include <Python.h>
#include "column.h"

typedef struct py_column
{
    PyObject_HEAD
    column* col;
    char* name;
} py_column;

static PyTypeObject py_column_Type;

static PyObject* py_print_column(py_column* a);
void tests();

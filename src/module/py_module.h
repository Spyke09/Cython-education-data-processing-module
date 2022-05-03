#include <Python.h>
#include <limits.h>
#include <float.h>
#include <string.h>
#include "column.h"
#include "vector.h"
#include "table.h"
#include <math.h>

typedef struct
{
    PyObject_HEAD
    table* dt;
} py_table;

typedef struct
{
    PyObject_HEAD
    column* col;
    //vector_char_t* name;
} py_column;

static PyTypeObject py_column_Type;
static PyTypeObject py_table_Type;

static PyObject* new_table(PyObject *self, PyObject *args);
static PyObject *py_print_table(PyObject* a, PyObject* args);
static PyObject* table_shape(PyObject* a, PyObject* args);
static void py_table_dealloc(py_table *self);

static PyObject* py_set_name_column(PyObject *self, PyObject *args);
static PyObject *py_print_column(PyObject* self, PyObject *args);

static py_column* py_column_init();
static PyObject* new_column(PyObject *self, PyObject *args);
static PyObject *len_of_py_column(PyObject *a, PyObject *args);
static void py_column_dealloc(py_column *self);
static PyObject *py_fill_column_from_list(PyObject *self, PyObject *args);
static void clear_py_column(py_column* self);


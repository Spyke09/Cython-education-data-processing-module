#include <Python.h>
#include "column.h"
#include "vector.h"

typedef struct
{
    PyObject_HEAD
    column* col;
    //vector_char_t* name;
    char* name;

} py_column;

static PyTypeObject py_column_Type;

static PyObject *py_print_column(py_column* self);
static PyObject* py_set_name_column(py_column *self, PyObject *args);
static py_column* py_column_init();
static PyObject* new_column(PyObject *self, PyObject *args);
static PyObject *py_fill_column_from_list(py_column *self, PyObject *args);
static void clear_py_column(py_column* self);


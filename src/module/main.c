#include <Python.h>
#include "column.h"


static PyObject *py_plus(PyObject *self, PyObject *args)
{
    double x, y;
    if (!PyArg_ParseTuple(args, "dd", &x, &y))
        return NULL;
    return Py_BuildValue("d", x+y);
}

static PyMethodDef ownmod_methods[] = {
{
    "plus",
    py_plus,
    METH_VARARGS,
    "Plus function"
},
{NULL, NULL, 0, NULL}
};

static PyModuleDef simple_module = {
    PyModuleDef_HEAD_INIT,
    "my_plus",
    "doc",
    -1,
    ownmod_methods
};

PyMODINIT_FUNC PyInit_my_plus(void)
{
    PyObject* m;
    m = PyModule_Create(&simple_module);
    if (m==NULL)
        return NULL;
    return m;
}

int main()
{
    char* str[] = {"23.1","90.234","89.987", "aboba", "0", "1"};
    column* res = get_column_str(str,6);
    type_column(&res, DOUBLE_TYPE);
    print_column(res);
    column* res_int = get_typed_column(&res, INT_TYPE);
    print_column(res_int);
    column* res_double = get_typed_column(&res, DOUBLE_TYPE);
    print_column(res_double);
    column* res_copy = get_typed_column(&res, STRING_TYPE);

    set_char(res_copy, 'A', 3, 0);
    print_column(res_copy);
    column* res_bool = get_typed_column(&res, BOOL_TYPE);
    print_column(res_bool);
    return 0;
}

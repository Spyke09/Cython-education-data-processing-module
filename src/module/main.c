#include "main.h"

static PyObject *some_tests(PyObject *self, PyObject *args)
{
    tests();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef ownmod_methods[] = {
{
    "some_tests",
    some_tests,
    METH_VARARGS,
    "Some tests"
},
{NULL, NULL, 0, NULL}
};

static PyModuleDef simple_module = {
    PyModuleDef_HEAD_INIT,
    "data_table",
    "doc",
    -1,
    ownmod_methods
};

PyMODINIT_FUNC PyInit_data_table(void)
{
    PyObject* m;
    m = PyModule_Create(&simple_module);
    if (m==NULL)
        return NULL;
    return m;
}

void tests()
{
    char* str[] = {"23.1","90.234","89.987", "aboba", "0", "1"};
    column* res = get_column_str(str, 6);
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
}

int main()
{
    tests();
    return 0;
}

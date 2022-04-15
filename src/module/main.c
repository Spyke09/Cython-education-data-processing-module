#include <Python.h>

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
    printf("aboba");
    return 0;
}

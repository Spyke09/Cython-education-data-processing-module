#include "py_module.h"

static PyObject* py_set_name_column(py_column *self, PyObject *args)
{
    char *temp;
    if (!PyArg_ParseTuple(args, "s", &temp))
        return NULL;
    v_init(self->col->name);
    for (int i = 0; temp[i]!='\0'&&i<LEN; ++i) v_push(self->col->name, temp[i]);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* new_column(PyObject *self, PyObject *args)
{
    int size;
    if (!PyArg_ParseTuple(args, "i", &size))
        return NULL;
    PyObject* temp = (PyObject*)py_column_init(size);
    return temp;
}

static py_column* py_column_init(int size)
{
    py_column *self;
    self = PyObject_NEW(py_column, &py_column_Type);
    self->col = create_column(INT_TYPE, size);
    char* tok = "unnamed";
    self->col->name = malloc(sizeof(vector_char_t));
    vector_char_t* temp = self->col->name;
    v_init(temp);
    for (int i = 0; tok[i]!='\0'&&i<LEN; ++i) v_push(temp, tok[i]);
    return self;
}

static PyObject *len_of_py_column(py_column *self)
{
    int size = self->col->len;
    return Py_BuildValue("i", size);
}

static PyObject *py_fill_column_from_list(py_column *self, PyObject *args)
{
    type_column(&self->col, DOUBLE_TYPE);
    PyObject *pList;
    PyObject *pItem;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be a list.");

        return NULL;
    }
    n = PyList_Size(pList);
    if (n!=(self->col)->len) {
        PyErr_SetString(PyExc_TypeError, "Invalid size");
        return NULL;
    }
    for (int i=0; i<n; i++) {
        pItem = PyList_GetItem(pList, i);
        double s = PyFloat_AsDouble(pItem);
        set_double(self->col, s, i);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_print_column(py_column* self, PyObject *args)
{
    printf("sdfsdfdsf\n");
    print_vec_str(self->col->name);
    print_column(self->col);
    Py_INCREF(Py_None);
    return Py_None;
}

static void py_column_dealloc(py_column *self)
{
    clear_column(&(self->col));
    Py_XDECREF(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject* new_table(PyObject *self, PyObject *args)
{
    char *filename;
    if (!PyArg_ParseTuple(args, "s", &filename))
        return NULL;
    py_table* temp;
    temp = PyObject_NEW(py_table, &py_table_Type);
    temp->dt = table_from_csv(filename, ',');
    return (PyObject*)temp;
}

static PyObject* table_shape(py_table* self)
{
    int cols = self->dt->len;
    int rows = self->dt->columns[0]->len;
    return Py_BuildValue("ii", rows, cols);
}

static PyObject *py_print_table(py_table* self)
{
    for (int i=0; i<self->dt->len; i++)
    {
        print_vec_str(self->dt->columns[i]->name);
        print_column(self->dt->columns[i]);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static void py_table_dealloc(py_table *self)
{
    for (int i=0; i<self->dt->len; i++)
        clear_column(&(self->dt->columns[i]));
    Py_XDECREF(self);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyMethodDef ownmod_methods[] = {
    {
        "new_col",
        new_column,
        METH_VARARGS,
        "New column"
    },
    {
        "new_table",
        new_table,
        METH_VARARGS,
        "New table"
    },
    {NULL, NULL, 0, NULL}
};

static PyMethodDef column_methods[] = {
    {
        "print_column",
        py_print_column,
        METH_VARARGS,
        "Print column"
    },
    {
        "set_name",
        py_set_name_column,
        METH_VARARGS,
        "Set name for column"
    },
    {
        "fill_from_list",
        py_fill_column_from_list,
        METH_VARARGS,
        "Fill column from list"
    },
    {
        "len",
        len_of_py_column,
        METH_VARARGS,
        "Len of column"
    },
    {NULL, NULL, 0, NULL}
};

static PyMethodDef table_methods[] = {
    {
        "print_table",
        py_print_table,
        METH_VARARGS,
        "Print table"
    },
    {
        "shape",
        table_shape,
        METH_VARARGS,
        "Shape of table"
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
    py_column_Type.tp_new = PyType_GenericNew;
    py_table_Type.tp_new = PyType_GenericNew;
	if(PyType_Ready(&py_column_Type) < 0 || PyType_Ready(&py_table_Type) < 0)
    	    return NULL;
    m = PyModule_Create(&simple_module);
    if (m==NULL)
        return NULL;

    Py_INCREF(&py_column_Type);
    Py_INCREF(&py_table_Type);
    return m;
}

static PyTypeObject py_column_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "py_column",
    .tp_basicsize = sizeof(py_column),
    .tp_dealloc = (destructor)py_column_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Column objects",
    .tp_methods = column_methods,
};


static PyTypeObject py_table_Type = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "py_table",
    .tp_basicsize = sizeof(py_table),
    .tp_dealloc = (destructor)py_table_dealloc,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Table objects",
    .tp_methods = table_methods,
};

#include "py_module.h"

static PyObject* py_set_name_column(PyObject *a, PyObject *args)
{
    py_column *self = (py_column*)a;
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
    PyObject* temp = (PyObject*)py_column_init(size, INT_TYPE);
    return temp;
}

static py_column* py_column_init(int size, TYPE type)
{
    py_column *self;
    self = PyObject_NEW(py_column, &py_column_Type);
    self->col = create_column(type, size);
    return self;
}

static PyObject *len_of_py_column(PyObject *a, PyObject *args)
{
    py_column *self = (py_column*)a;
    int size = self->col->len;
    return Py_BuildValue("i", size);
}

static PyObject *py_fill_column_from_list(PyObject *a, PyObject *args)
{
Py_INCREF(Py_None);
    py_column* self = (py_column*)a;
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
    const char* st = PyList_GetItem(pList, 0)->ob_type->tp_name;
    if (strcmp("int", st)==0) {
        type_column(&self->col, INT_TYPE);
        for (int i=0; i<n; i++) {
            pItem = PyList_GetItem(pList, i);
            int s = PyLong_AsLong(pItem);
            set_int(self->col, s, i);
        }
    } else if (strcmp("float", st)==0) {
        type_column(&self->col, DOUBLE_TYPE);
        for (int i=0; i<n; i++) {
            pItem = PyList_GetItem(pList, i);
            double s = PyFloat_AsDouble(pItem);
            set_double(self->col, s, i);
        }
    } else if (strcmp("str", st)==0) {
        type_column(&self->col, STRING_TYPE);
        for (int i=0; i<n; i++) {
            pItem = PyList_GetItem(pList, i);
            PyObject *encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
            char* s = PyBytes_AsString(encodedString);
            set_str(self->col, s, i);
        }
    } else if (strcmp("bool", st)==0) {
        type_column(&self->col, BOOL_TYPE);
        for (int i=0; i<n; i++) {
            pItem = PyList_GetItem(pList, i);
            int s = PyObject_IsTrue(pItem);
            set_bool(self->col, s, i);
        }
    }
    return Py_None;
}

static PyObject *py_print_column(PyObject* a, PyObject *args)
{
    py_column* self = (py_column*)a;
    print_column(self->col);
    Py_INCREF(Py_None);
    return Py_None;
}

static void py_column_dealloc(py_column *self)
{
    clear_column(self->col);
    free(self->col);
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

static PyObject* table_shape(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    int cols = self->dt->len;
    int rows = self->dt->columns[0]->len;
    return Py_BuildValue("ii", rows, cols);
}

static PyObject* get_column_from_name(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    int ind = -1;
    char* name;

    if (!PyArg_ParseTuple(args, "s", &name)) return NULL;
    vector_char_t v;
    v_init(&v);
    str_to_vec(&v, name);
    for (int i = 0; i< self->dt->len; ++i)
        if (str_vec_equal(&v, self->dt->columns[i]->name))
        {
            ind = i;
            break;
        }
    v_clear(&v);

    column* temp = self->dt->columns[ind];
    py_column* result = py_column_init(temp->len, temp->type);
    set_name_from_vec(result->col, temp->name);
    if (temp->type==STRING_TYPE)
        for (int i = 0; i<temp->len; ++i) set_vec_str(result->col, get_str(temp, i), i);
    if (temp->type==BOOL_TYPE)
        for (int i = 0; i<temp->len; ++i) set_bool(result->col, get_bool(temp, i), i);
    if (temp->type==DOUBLE_TYPE)
        for (int i = 0; i<temp->len; ++i) set_double(result->col, get_double(temp, i), i);
    if (temp->type==INT_TYPE)
        for (int i = 0; i<temp->len; ++i) set_int(result->col, get_int(temp, i), i);
    return (PyObject*)result;
}

static PyObject* get_column_from_ind(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    int ind = -1;
    if (!PyArg_ParseTuple(args, "i", &ind)) return NULL;

    column* temp = self->dt->columns[ind];
    py_column* result = py_column_init(temp->len, temp->type);
    set_name_from_vec(result->col, temp->name);
    if (temp->type==STRING_TYPE)
        for (int i = 0; i<temp->len; ++i) set_vec_str(result->col, get_str(temp, i), i);
    if (temp->type==BOOL_TYPE)
        for (int i = 0; i<temp->len; ++i) set_bool(result->col, get_bool(temp, i), i);
    if (temp->type==DOUBLE_TYPE)
        for (int i = 0; i<temp->len; ++i) set_double(result->col, get_double(temp, i), i);
    if (temp->type==INT_TYPE)
        for (int i = 0; i<temp->len; ++i) set_int(result->col, get_int(temp, i), i);
    return (PyObject*)result;
}

static PyObject* set_column_from_ind(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    int ind = -1;
    PyObject *pList;
    PyObject *pItem;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
        return NULL;
    }
    pItem = PyList_GetItem(pList, 1);
    ind = PyLong_AsLong(pItem);
    pItem = PyList_GetItem(pList, 0);
    py_column* added_c = (py_column*)pItem;
    column** cc = &(self->dt->columns[ind]);
    if(self->dt->len<=ind) return NULL;
    clear_column(*cc);
    *cc = create_column(added_c->col->type, added_c->col->len);
    column* c = *cc;
    set_name_from_vec(c, added_c->col->name);
    if (added_c->col->type==STRING_TYPE)
        for (int i = 0; i<c->len; ++i) set_vec_str(c, get_str(added_c->col, i), i);
    if (added_c->col->type==BOOL_TYPE)
        for (int i = 0; i<c->len; ++i) set_bool(c, get_bool(added_c->col, i), i);
    if (added_c->col->type==DOUBLE_TYPE)
        for (int i = 0; i<c->len; ++i) set_double(c, get_double(added_c->col, i), i);
    if (added_c->col->type==INT_TYPE)
        for (int i = 0; i<c->len; ++i) set_int(c, get_int(added_c->col, i), i);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *py_print_table(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    for (int i=0; i<self->dt->len; i++)
        print_column(self->dt->columns[i]);
    Py_INCREF(Py_None);
    return Py_None;
}

static void py_table_dealloc(py_table *self)
{
    for (int i=0; i<self->dt->len; i++)
    {
        clear_column(self->dt->columns[i]);
    }
    free(self->dt->columns);
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
    {
        "column_from_name",
        get_column_from_name,
        METH_VARARGS,
        "Getter of columns"
    },
    {
        "column_from_index",
        get_column_from_ind,
        METH_VARARGS,
        "Getter of columns"
    },
    {
        "set_column",
        set_column_from_ind,
        METH_VARARGS,
        "Setter of columns"
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

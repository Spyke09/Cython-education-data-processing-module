#include "py_module.h"

static PyObject* py_set_name_column(py_column *self, PyObject *args)
{
    if (!PyArg_ParseTuple(args, "s", &self->name))
            return NULL;
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
    char* a = "unnamed";
    self->name = a;
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

static void clear_py_column(py_column* self)
{
    clear_column(&(self->col));
}

static PyObject *py_print_column(py_column* self)
{
    printf("%s\n", self->name);
    print_column(self->col);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyMethodDef ownmod_methods[] = {
    {
        "new_col",
        new_column,
        METH_VARARGS,
        "New column"
    },
    {NULL, NULL, 0, NULL}
};

static void py_column_dealloc(py_column *self)
{
    clear_py_column(self);
    Py_XDECREF(&self->col);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

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
	if(PyType_Ready(&py_column_Type) < 0)
    	    return NULL;
    m = PyModule_Create(&simple_module);
    if (m==NULL)
        return NULL;
    Py_INCREF(&py_column_Type);
    return m;
}

static PyTypeObject py_column_Type = {
     /* Everything about object */
     PyVarObject_HEAD_INIT(NULL, 0)
     //tp_name – имя типа
     "py_column",         	/* tp_name */
     //tp_basicsize – размер объекта, если размер типа не изменяем
      sizeof(py_column), /* tp_basicsize */
      /*tp_itemsize – если размер типа изменяем,
      то размер объекта = tp_basicsize + N* tp_itemsize, где N – “длина” объекта.
      */
    	0,                   	/* tp_itemsize */
//tp_dealloc – деструктор
	(destructor)py_column_dealloc,                   	/* tp_dealloc */
	0,                   	/* tp_print */
	0,                   	/* tp_getattr */
	0,                   	/* tp_setattr */
	0,                   	/* tp_reserved */
	0,                   	/* tp_repr */
	0,                   	/* tp_as_number */
	0,                	   /* tp_as_sequence */
	0,                   	/* tp_as_mapping */
	0,                   	/* tp_hash */
	0,                   	/* tp_call */
	0,                   	/* tp_str */
	0,                   	/* tp_getattro */
	0,                       /* tp_setattro */
	0,                   	/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,    	/* tp_flags */
        //tp_doc -строка документации
	"Column objects",       	/* tp_doc: test.Test.__doc__ */
	0,                   	/* tp_traverse */
	0,                       /* tp_clear */
	0,                   	/* tp_richcompare */
	0,                   	/* tp_weaklistoffset */
	0,                   	/* tp_iter */
	0,                   	/* tp_iternext */
       //tp_methods – указатель на структуру с методами класса
	column_methods,     	 /* tp_methods */
	0,                   	/* tp_members */
	0,                   	/* tp_getset */
	0,                   	/* tp_base */
	0,                   	/* tp_dict */
	0,                   	/* tp_descr_get */
	0,                   	/* tp_descr_set */
	0,                   	/* tp_dictoffset */
       //tp_init – указатель на конструктор
	0,   /* tp_init */
	0,                   	/* tp_alloc */
	0,                   	/* tp_new */
};
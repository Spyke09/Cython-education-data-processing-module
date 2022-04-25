#include "py_module.h"

static PyObject* py_set_name_column(py_column *self, PyObject *args)
{
//    char *temp;
//    if (!PyArg_ParseTuple(args, "s", &temp))
//        return NULL;
//    str_to_vec(self->name, temp);
    if (!PyArg_ParseTuple(args, "s", &self->name))
            return NULL;
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* new_column(PyObject *self, PyObject *args)
{
    PyObject* temp = (PyObject*)py_column_init();
    return temp;
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
//        if(!PyString_Check(pItem)) {
//            PyErr_SetString(PyExc_TypeError, "list items must be integers.");
//            return NULL;
//        }
        double s = PyFloat_AsDouble(pItem);
        set_double(self->col, s, i);
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static void clear_py_column(py_column* self)
{
//    v_clear(self->name);
    clear_column(&(self->col));
}

static PyObject *some_tests()
{
    tests();
    Py_INCREF(Py_None);
    return Py_None;
}


static PyObject *py_print_column(py_column* self)
{
//    char* temp_c = vec_to_str(self->name);
//    printf("%s\n", temp_c);
//    free(temp_c);
    printf(self->name);
    print_column(self->col);
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
    {
        "new_col",
        new_column,
        METH_VARARGS,
        "New column"
    },
    {NULL, NULL, 0, NULL}
};

static py_column* py_column_init()
{
    py_column *self;
    self = PyObject_NEW(py_column, &py_column_Type);
    self->col = create_column(INT_TYPE, 10);
//    vector_char_t* t = malloc(sizeof(vector_char_t*));
//    v_init(t);
//    self->name = t;
    char* a = "unnamed";
//    str_to_vec(t, a);
    printf("column init\n");
    return self;
}

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

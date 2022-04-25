#include "py_module.h"

static PyObject *some_tests()
{
    tests();
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* py_print_column(py_column* self)
{
    printf("%s\n",self->name);
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
        "print_column",
        py_print_column,
        METH_VARARGS,
        "Print column"
    },
    {NULL, NULL, 0, NULL}
};

static int py_column_init(py_column *self, PyObject *args, PyObject *kwds)
{
    printf("column init\n");
    self->col = create_column(INT_TYPE, 10);
    self->name = "unnamed";
    return 0;
}

static void py_column_dealloc(py_column* self)
{
    clear_column(&(self->col));
    Py_XDECREF(self->col);
    free(self->name);
    Py_TYPE(self)->tp_free((PyObject*)self);
}

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
	PyModule_AddObject(m, "crea", (PyObject *)&py_column_Type);
    return m;
}

static PyTypeObject py_column_Type = {
     /* Everything about object */
     PyVarObject_HEAD_INIT(NULL, 0)
     //tp_name � ��� ����
     "py_column",         	/* tp_name */
     //tp_basicsize � ������ �������, ���� ������ ���� �� ��������
      sizeof(py_column), /* tp_basicsize */
      /*tp_itemsize � ���� ������ ���� ��������,
      �� ������ ������� = tp_basicsize + N* tp_itemsize, ��� N � ������� �������.
      */
    	0,                   	/* tp_itemsize */
//tp_dealloc � ����������
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
        //tp_doc -������ ������������
	"Column object",       	/* tp_doc: test.Test.__doc__ */
	0,                   	/* tp_traverse */
	0,                       /* tp_clear */
	0,                   	/* tp_richcompare */
	0,                   	/* tp_weaklistoffset */
	0,                   	/* tp_iter */
	0,                   	/* tp_iternext */
       //tp_methods � ��������� �� ��������� � �������� ������
	ownmod_methods,     	 /* tp_methods */
	0,                   	/* tp_members */
	0,                   	/* tp_getset */
	0,                   	/* tp_base */
	0,                   	/* tp_dict */
	0,                   	/* tp_descr_get */
	0,                   	/* tp_descr_set */
	0,                   	/* tp_dictoffset */
       //tp_init � ��������� �� �����������
	(initproc)py_column_init,   /* tp_init */
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

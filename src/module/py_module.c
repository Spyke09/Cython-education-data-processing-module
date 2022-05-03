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

static PyObject* py_print_table(PyObject* a, PyObject *args)
{
    int n;
    if (!PyArg_ParseTuple(args, "i", &n)) return NULL;
    py_table* self = (py_table*)a;
    print_table(self->dt, n);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* py_print_column_types(PyObject* a, PyObject *args)
{
    py_table* self = (py_table*)a;
    print_column_types(self->dt);
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject* py_head_table(PyObject* a, PyObject *args)
{
    py_table* self = (py_table*)a;
    print_table(self->dt, 10);
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

static PyObject* py_get_max_value(PyObject* a, PyObject* args)
{
    py_column* self = (py_column*)a;
    switch(self->col->type)
    {
        case INT_TYPE:
            int max_i = INT_MIN;
            for (int i = 0; i < self->col->len; i++)
            {
                if (get_int(self->col, i) > max_i) max_i = get_int(self->col, i);
            }
            return Py_BuildValue("i", max_i);
                break;
        case DOUBLE_TYPE:
            double max_d = DBL_MIN;
            for (int i = 0; i < self->col->len; i++)
            {
                if (get_double(self->col, i) > max_d) max_d = get_double(self->col, i);
            }
            return Py_BuildValue("d", max_d);
            break;
        case BOOL_TYPE:
            return Py_BuildValue("i", 1);
            break;
        case STRING_TYPE:
            char* max_s = "";
            for (int i = 0; i < self->col->len; i++)
            {
                char* st = vec_to_str(get_str(self->col, i));
                if (strlen(st) > strlen(max_s)) max_s = st;
                free(st);
            }
            return Py_BuildValue("s", max_s);
            break;
    }
}

static PyObject* py_get_min_value(PyObject* a, PyObject* args)
{
    py_column* self = (py_column*)a;
    switch(self->col->type)
    {
        case INT_TYPE:
            int min_i = INT_MAX;
            for (int i = 0; i < self->col->len; i++)
            {
                if (get_int(self->col, i) < min_i) min_i = get_int(self->col, i);
            }
            return Py_BuildValue("i", min_i);
                break;
        case DOUBLE_TYPE:
            double min_d = DBL_MAX;
            for (int i = 0; i < self->col->len; i++)
            {
                if (get_double(self->col, i) < min_d) min_d = get_double(self->col, i);
            }
            return Py_BuildValue("d", min_d);
            break;
        case BOOL_TYPE:
            return Py_BuildValue("i", 0);
            break;
        case STRING_TYPE:
            char* min_s = vec_to_str(get_str(self->col, 0));
            for (int i = 0; i < self->col->len; i++)
            {
                char* st = vec_to_str(get_str(self->col, i));
                if (strlen(st) < strlen(min_s)) min_s = st;
                free(st);
            }
            return Py_BuildValue("s", min_s);
            break;
    }
}

static PyObject* py_insert_column(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    py_column* ins_col;
    if (!PyArg_ParseTuple(args, "O!", &py_column_Type, &ins_col)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
        return NULL;
    }
    column** res = (column**)malloc((self->dt->len + 1)*sizeof(column*));
    Py_INCREF(Py_None);
    if (self->dt->columns[0]->len == ins_col->col->len)
    {
        for (int i = 0; i < self->dt->len; i++)
        {
            res[i] = self->dt->columns[i];
        }
        res[self->dt->len] = copy_column(ins_col->col);
        free(self->dt->columns);
        self->dt->columns = res;
        self->dt->len++;
        return Py_None;
    }
    else return Py_None;
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
    py_column* added_c;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "iO!", &ind, &py_column_Type, &added_c)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
        return NULL;
    }
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

PyObject* set_types(PyObject* a, PyObject* args)
{
    Py_INCREF(Py_None);
    py_table* self = (py_table*)a;
    PyObject *pList;
    PyObject *pItem;
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "O!", &PyList_Type, &pList)) {
        PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
        return NULL;
    }
    n = PyList_Size(pList);
    if (n!=(self->dt->len)) {
        PyErr_SetString(PyExc_TypeError, "Invalid size");
        return NULL;
    }
    for (int i = 0; i<n; ++i)
    {
        pItem = PyList_GetItem(pList, i);
        PyObject *encodedString = PyUnicode_AsEncodedString(pItem, "UTF-8", "strict");
        char* s = PyBytes_AsString(encodedString);
        if (strcmp("int", s)==0) type_column(&(self->dt->columns[i]), INT_TYPE);
        if (strcmp("float", s)==0) type_column(&(self->dt->columns[i]), DOUBLE_TYPE);
        if (strcmp("bool", s)==0) type_column(&(self->dt->columns[i]), BOOL_TYPE);
        if (strcmp("str", s)==0) type_column(&(self->dt->columns[i]), STRING_TYPE);
    }
    return Py_None;
}

double median3(double a, double b, double c)
{
    if ((a<=b && a>=c) || (a<=c && a>=b)) return  a;
    if ((b<=c && b>=a) || (b<=a && b>=c)) return  b;
    if ((c<=a && c>=b) || (c<=b && c>=a)) return  c;
}

void insertionSort(table *dt, int first, int last, int key, TYPE type)
{
    for (int t = first; t<last+1; ++t)
    {
        int j = t - 1;
        if (type==INT_TYPE)
            while(j>=0 && get_int(dt->columns[key],j) > get_int(dt->columns[key],j+1))
            {
                for (int i = 0; i<dt->len; ++i)
                {
                    void* temp = dt->columns[i]->values[j];
                    dt->columns[i]->values[j] = dt->columns[i]->values[j+1];
                    dt->columns[i]->values[j+1] = temp;
                }
                j--;
            }
        if (type==DOUBLE_TYPE)
            while(j>=0 && get_double(dt->columns[key],j) > get_double(dt->columns[key],j+1))
            {
                for (int i = 0; i<dt->len; ++i)
                {
                    void* temp = dt->columns[i]->values[j];
                    dt->columns[i]->values[j] = dt->columns[i]->values[j+1];
                    dt->columns[i]->values[j+1] = temp;
                }
                j--;
            }
    }
}

void quickSort(table *dt, int first, int last, int key, TYPE type)
{
    if (last - first < 11){
        insertionSort(dt, first, last, key, type);
        return;
    }

    int left = first, right = last;
    int middle = (left+right)/2;
    double a, b, c;
    if (type==INT_TYPE){
        a = get_int(dt->columns[key],left);
        c = get_int(dt->columns[key],middle);
        b = get_int(dt->columns[key],right);
    }
    if (type==DOUBLE_TYPE){
        a = get_double(dt->columns[key],left);
        c = get_double(dt->columns[key],middle);
        b = get_double(dt->columns[key],right);
    }
    double median = median3(a, b, c);
    while(left<=right)
    {
        if (type==INT_TYPE)
        {
            while (get_int(dt->columns[key],left) < median) left++;
            while (get_int(dt->columns[key],right) > median) right--;
        }
        else if (type==DOUBLE_TYPE)
        {
            while (get_double(dt->columns[key],left) < median) left++;
            while (get_double(dt->columns[key],right) > median) right--;
        }
        if (left >= right) break;
        for (int i = 0; i<dt->len; ++i)
        {
            void* t = dt->columns[i]->values[left];
            dt->columns[i]->values[left] = dt->columns[i]->values[right];
            dt->columns[i]->values[right] = t;

        }
        left++;
        right--;
    }
    quickSort(dt, first, right, key, type);
    quickSort(dt, right+1, last, key, type);
}

static PyObject* sort_table(PyObject* a, PyObject* args)
{
    py_table* self = (py_table*)a;
    int key;
    if (!PyArg_ParseTuple(args, "i", &key))
        return NULL;
    TYPE type = self->dt->columns[key]->type;
    Py_INCREF(Py_None);
    if (type!=DOUBLE_TYPE && type!=INT_TYPE) return Py_None;
    table* old_t = self->dt;
    table* new_t = copy_table(old_t);
    quickSort(new_t, 0, new_t->columns[0]->len-1, key, type);
    py_table* result = PyObject_NEW(py_table, &py_table_Type);
    result->dt = new_t;
    return (PyObject*)result;
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
        "print",
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
    {
        "max",
        py_get_max_value,
        METH_VARARGS,
        "Getter of maxvalue of a column"
    },
    {
        "min",
        py_get_min_value,
        METH_VARARGS,
        "Getter of minvalue of a column"
    },
    {NULL, NULL, 0, NULL}
};

static PyMethodDef table_methods[] = {
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
    {
        "print",
        py_print_table,
        METH_VARARGS,
        "Print table"
    },
    {
        "head",
        py_head_table,
        METH_VARARGS,
        "Head"
    },
    {
        "print_column_types",
        py_print_column_types,
        METH_VARARGS,
        "Print column types"
    },
    {
        "insert",
        py_insert_column,
        METH_VARARGS,
        "Insert column"
    },
    {
        "sort_by",
        sort_table,
        METH_VARARGS,
        "Sort table"
    },
    {
        "set_types",
        set_types,
        METH_VARARGS,
        "Set types table"
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

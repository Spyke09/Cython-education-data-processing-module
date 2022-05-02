#include "column.h"

void set_name_from_str(column* b, char* name)
{
    v_clear(b->name);
    for (int i = 0; name[i]!='\0' && i<MAX_STR_LEN; i++) v_push(b->name, name[i]);
}

void set_name_from_vec(column* b, vector_char_t* name)
{
    v_clear(b->name);
    for (int i = 0; i<name->len; i++) v_push(b->name, name->data[i]);
}

// сеттеры для каждого типа для абстргирования от ссылок и указателей
void set_vec_str_with_del(column* a, vector_char_t* value, int ind)
{
    free(((vector_char_t*)(a->values[ind]))->data);
    free(a->values[ind]);
    *(vector_char_t*)(a->values[ind]) = *value;
}

void set_vec_str(column* a, vector_char_t* value, int ind)
{
    v_clear((vector_char_t*)(a->values[ind]));
    for (int i = 0; i<value->len; ++i) v_push((vector_char_t*)(a->values[ind]), value->data[i]);
}

void set_str(column* a, char* value, int ind)
{
    v_clear(((vector_char_t*)(a->values[ind])));
    str_to_vec(a->values[ind], value);
}

void set_char(column* a, char value, int ind, int jnd)
{
    ((vector_char_t*)(a->values[ind]))->data[jnd] = value;
}

void set_int(column* a, int value, int ind)
{
    *(int*)(a->values[ind]) = value;
}

void set_double(column* a, double value, int ind)
{
    *(double*)(a->values[ind]) = value;
}

void set_bool(column* a, int value, int ind)
{
    *(int*)(a->values[ind]) = value!=0;
}

// геттеры аналогично
vector_char_t* get_str(column* a, int ind)
{
    return (vector_char_t*)(a->values[ind]);
}

int get_int(column* a, int ind)
{
    return *(int*)(a->values[ind]);
}

double get_double(column* a, int ind)
{
    return *(double*)(a->values[ind]);
}

int get_bool(column* a, int ind)
{
    return *(int*)(a->values[ind]);
}

// вектор-строку в сишную строку
// НАДО ОСВОБОЖДАТЬ ПАМЯТЬ!!!
char* vec_to_str(vector_char_t* v)
{
    char* res = malloc(sizeof(char)*(v->len+1));
    for (int i = 0; i<v->len; ++i) res[i] = v->data[i];
    res[v->len] = '\0';
    return res;
}

// поместить содердимое сишной строки в вектор-строку
void str_to_vec(vector_char_t* t, char* c)
{
    v_clear(t);
    for(int i = 0; (c[i]!='\0')&&(i<MAX_STR_LEN); ++i) v_push(t, c[i]);
}

// размеры каждого типа в байтах
int type_size(TYPE type)
{
    switch (type)
    {
    case INT_TYPE:
        return sizeof(int);
    case BOOL_TYPE:
        return sizeof(int);
    case DOUBLE_TYPE:
        return sizeof(double);
    case STRING_TYPE:
        return sizeof(vector_char_t);
    }
    return -1;
}

// создание пустой колонки
column* create_column(TYPE a, int n)
{
    column* res = (column*)malloc(sizeof(column));
    res->type = a;
    res->size = type_size(a);
    res->len = n;
    res->values = malloc(n*sizeof(void*));
    res->name = malloc(sizeof(vector_char_t));
    v_init(res->name);
    set_name_from_str(res, "unnamed");
    switch(a)
    {
        case INT_TYPE:
            for (int i = 0; i<n; ++i){
                int* t = malloc(res->size);
                *t = 0;
                res->values[i] = t;
            }
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<n; ++i){
                double* t = malloc(res->size);
                *t = 0;
                res->values[i] = t;
            }
            break;
        case BOOL_TYPE:
            for (int i = 0; i<n; ++i){
                int* t = malloc(res->size);
                *t = 0;
                res->values[i] = t;
            }
            break;
        case STRING_TYPE:
            for (int i = 0; i<n; ++i){
                vector_char_t* t = malloc(res->size);
                v_init(t);
                res->values[i] = t;
            }
            break;
    }
    return res;
}

// печать колонки
void print_column(column* a)
{
    int ot = 10;
    print_vec_str(a->name);
    switch(a->type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i)
                printf("%*d", ot, get_int(a, i));

            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i) {
                printf("%*f", ot, get_double(a, i));
            }
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i) {
                printf("%*s", ot, get_bool(a, i)?"true":"false");
            }
            break;
        case STRING_TYPE:
            for (int i = 0; i<a->len; ++i) {
                char* st = vec_to_str(get_str(a,i));
                printf("%*s",ot, st);
                free(st);
            }
            break;
    }
    printf("\n");
}

// получение новой колонки из массива сишных строк
column* get_column_str(char** str, int len)
{
    column* a = create_column(STRING_TYPE, len);
    for (int i = 0; i<a->len; ++i) {
        vector_char_t* t = get_str(a,i);
        for (int j = 0; (str[i][j]!='\0')&&(j<MAX_STR_LEN); j++) v_push(t, str[i][j]);
    }
    return a;
}

// метод освобождения памяти, занимаемой колонкой
void clear_column(column* a)
{
    v_clear(a->name);
    free(a->name);
    if (a->type==STRING_TYPE)
        for (int i = 0; i<a->len; ++i){
            vector_char_t* t = get_str(a, i);
            v_clear(t);
        }
    for (int i = 0; i<a->len; ++i) free(a->values[i]);
    free(a->values);
}

// далее методы преобразования
// из ветора-строки в type
column* get_typed_column_from_str(column** an, TYPE type)
{
    column* a = *an;
    column* b = create_column(type, a->len);
    set_name_from_vec(b, a->name);
    switch(type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i) {
                vector_char_t* str = get_str(a, i);
                char* temp_c = vec_to_str(str);
                int t = atoi(temp_c);
                free(temp_c);
                set_int(b, t, i);
            }
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i) {
                vector_char_t* str = get_str(a, i);
                char* temp_c = vec_to_str(str);
                double t = atof(temp_c);
                set_double(b, t, i);
            }
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i) {
                vector_char_t* str = get_str(a, i);
                char* temp_c = vec_to_str(str);
                int t = !(*temp_c=='0'&&temp_c[1]=='\0');
                set_bool(b, t, i);
            }
            break;
        case STRING_TYPE:
            for (int i = 0; i<a->len; ++i) {
                int t;
                v_clone(get_str(a, i), get_str(b, i), t);
            }
    }
    return b;
}

// из int в type
column* get_typed_column_from_int(column** an, TYPE type)
{
    column* a = *an;
    column* b = create_column(type, a->len);
    set_name_from_vec(b, a->name);
    switch(type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_int(b, get_int(a, i), i);
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_double(b, get_int(a, i), i);
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_bool(b, get_int(a, i), i);
            break;
        case STRING_TYPE:
            for (int i = 0; i<a->len; ++i) {
                char* temp = malloc(sizeof(char)*MAX_STR_LEN);
                sprintf(temp, "%d", get_int(a, i));
                str_to_vec(get_str(b, i), temp);
                free(temp);
            }
    }
    return b;
}

// из double в type
column* get_typed_column_from_double(column** an, TYPE type)
{
    column* a = *an;
    column* b = create_column(type, a->len);
    set_name_from_vec(b, a->name);
    switch(type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_int(b, (int)get_double(a, i), i);
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_double(b, get_double(a, i), i);
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_bool(b, (int)get_double(a, i), i);
            break;
        case STRING_TYPE:
            for (int i = 0; i<a->len; ++i) {
                char* temp = malloc(sizeof(char)*MAX_STR_LEN);
                sprintf(temp, "%f", get_double(a, i));
                str_to_vec(get_str(b, i), temp);
                free(temp);
            }
    }
    return b;
}

// из любого типа в type
column* get_typed_column(column** an, TYPE type)
{
    column* a = *an;
    switch (a->type)
    {
        case STRING_TYPE:
            return get_typed_column_from_str(an, type);
        case INT_TYPE:
            return get_typed_column_from_int(an, type);
        case BOOL_TYPE:
            return get_typed_column_from_int(an, type);
        case DOUBLE_TYPE:
            return get_typed_column_from_double(an, type);
    }
    return a;
}

// сменить тип у подаваемой на вход колонки
void type_column(column** an, TYPE type)
{
    column* b = get_typed_column(an, type);
    clear_column(*an);
    free(*an);
    *an = b;
}

void print_vec_str(vector_char_t* t)
{
    for (int i = 0; i<t->len; ++i) printf("%c", t->data[i]);
    printf("\n");
}

int str_vec_equal(vector_char_t* a, vector_char_t* b)
{
    if (a->len!=b->len) return 0;
    for (int i = 0; i<a->len; ++i) if (a->data[i]!=b->data[i]) return 0;
    return 1;
}

column* copy_column(column* a)
{
    column* b = create_column(a->type, a->len);
    set_name_from_vec(b, a->name);
    switch(a->type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i) set_int(b, get_int(a, i), i);
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i) set_bool(b, get_bool(a, i), i);
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i) set_double(b, get_double(a, i), i);
            break;
        case STRING_TYPE:
            for (int i = 0; i<a->len; ++i) set_vec_str(b, get_str(a, i), i);
            break;
    }
    return b;
}

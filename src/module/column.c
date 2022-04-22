#include "column.h"

// сеттеры для каждого типа для абстргирования от ссылок и указателей
void set_str(column* a, vector_char_t* value, int ind)
{
    free(((vector_char_t*)(a->values[ind]))->data);
    free(a->values[ind]);
    *(vector_char_t*)(a->values[ind]) = *value;
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
void clear_column(column** a)
{
    if ((*a)->type==STRING_TYPE)
        for (int i = 0; i<(*a)->len; ++i){
            vector_char_t* t = get_str(*a, i);
            free(t->data);
        }
    for (int i = 0; i<(*a)->len; ++i) free((*a)->values[i]);
    free((*a)->values);
    free(*a);
}

// далее методы преобразования
// из ветора-строки в type
column* get_typed_column_from_str(column** an, TYPE type)
{
    column* a = *an;
    column* b = create_column(type, a->len);
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
    switch(type)
    {
        case INT_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_int(b, get_double(a, i), i);
            break;
        case DOUBLE_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_double(b, get_double(a, i), i);
            break;
        case BOOL_TYPE:
            for (int i = 0; i<a->len; ++i)
                set_bool(b, get_double(a, i), i);
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
    clear_column(an);
    *an = b;
}

#include <stdio.h>
#include <stdlib.h>

typedef enum 
{
    INT_TYPE,
    DOUBLE_TYPE,
    BOOL_TYPE,
    STRING_TYPE
} TYPE;

typedef struct 
{
    void** values;
    TYPE type;
    int size;
    int len;
} column;

typedef struct 
{
    column* columns;
    int rows_size;
} table;


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
        return sizeof(char*);
    }
}

column* create_column(TYPE a, int n)
{
    column* res = malloc(sizeof(column));
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
                char** t = malloc(type_size(a));
                res->values[i] = t;
            }
            break;
    }
    return res;
}

void print_column(column* a)
{
    if (a->type==INT_TYPE) {
        for (int i = 0; i<a->len; ++i) {
            int* t = a->values[i];
            printf("%d ", *t);
        }
    } else if (a->type==DOUBLE_TYPE) {
            for (int i = 0; i<a->len; ++i) {
            double* t = a->values[i];
            printf("%f ", *t);
        }
    }
    if (a->type==STRING_TYPE){
        for (int i = 0; i<a->len; ++i) {
        double* t = a->values[i];
        printf("%s ", *t);
        }   
    }
    printf("\n");
}

int main()
{
    column* a = create_column(INT_TYPE, 10);
    print_column(a);
    a = create_column(DOUBLE_TYPE, 10);
    print_column(a);
    a = create_column(STRING_TYPE, 10);
    //print_column(a);
    return 0;
}
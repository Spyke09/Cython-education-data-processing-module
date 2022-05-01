#include "table.h"

int get_n_rows(char* filename)
{
    char str[LEN];
    FILE *file;
    file = fopen(filename,"r");
    int res = 0;
    if (file != NULL)
    {
        while (fgets(str, LEN, file)) res++;
        fclose(file);
    }
    else
    {
        fprintf(stderr, "File not found.");
    }
    return res;
}

int get_n_columns(char* filename, char del)
{
    char str[LEN];
    FILE *file;
    file = fopen(filename,"r");
    int res = 0;
    char delim[2];
    delim[0] = del;
    delim[1] = '\0';
    if (file != NULL)
    {
        fgets(str, LEN, file);
        char* tok = strtok(str, delim);
        while (tok != NULL) {
            res++;
            tok = strtok(NULL, delim);
        }
        fclose(file);
    }
    else
    {
        fprintf(stderr, "File not found.");
    }
    return res;
}

table* table_from_csv(char* filename, char del)
{
    char str[LEN];
    char delim[3];
    delim[0] = del;
    delim[1] = '\n';
    delim[2] = '\0';
    int rows = get_n_rows(filename);
    int columns = get_n_columns(filename, del);
    FILE *file;
    table* data;
    data = (table*)malloc(sizeof(table));
    data->len = columns;
    data->columns = (column**)malloc(columns*sizeof(column*));
    char* tok;
    for (int i = 0; i < columns; i++){
        data->columns[i] = create_column(STRING_TYPE, rows-1);
    }
    file = fopen(filename,"r");
    fgets(str, LEN, file);
    tok = strtok(str, delim);
    for (int j = 0; j<columns && tok != NULL; j++)
    {
        data->columns[j]->name = malloc(sizeof(vector_char_t));
        vector_char_t* temp = data->columns[j]->name;
        v_init(temp);
        for (int i = 0; tok[i]!='\0'&&i<LEN; ++i) v_push(temp, tok[i]);
        tok = strtok(NULL, delim);
    }

    int i = 0;
    if (file != NULL)
    {
        while (fgets(str, LEN, file) && (i < rows-1))
        {
            tok = strtok(str, delim);
            for (int j = 0; j<columns && tok != NULL; j++)
            {
                set_str(data->columns[j], tok, i);
                tok = strtok(NULL, delim);
            }
            i++;
        }
        fclose(file);
    }
    else
    {
        fprintf(stderr, "File not found.");
    }
    return data;
}

void print_table(table* table, int number)
{
    int ot = 20;
    number = min(number, table->columns[0]->len);
    printf("\n");
    for (int n = 0;n < table->len; n++)
    {
        vector_char_t* v = table->columns[n]->name;
        char* st = vec_to_str(v);
        printf("%*s", ot, st);
        free(st);
    }
    printf("\n");
    for (int i=0;i<ot*(table->len);i++) printf("_");
    printf("\n");

    for (int i = 0; i<number; i++)
    {
        for (int j = 0; j<table->len; j++)
        {
            switch (table->columns[j]->type)
            {
                case INT_TYPE:
                    printf("%*d", ot, get_int(table->columns[j], i));
                    break;
                case DOUBLE_TYPE:
                    printf("%*f", ot, get_double(table->columns[j], i));
                    break;
                case BOOL_TYPE:
                    printf("%*s", ot, get_bool(table->columns[j], i) ? "true" : "false");
                    break;
                case STRING_TYPE:
                    char* st = vec_to_str(get_str(table->columns[j],i));
                    printf("%*s", ot, st);
                    free(st);
                    break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

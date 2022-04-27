#include "table.h"

table* read_csv(int rows, int columns, char* filename, char* delim)
{
    char str[LEN];
    FILE *file;
    table* data;
    data = (table*)malloc(sizeof(table));
    data->columns = (column**)malloc(columns*sizeof(column*));
    for (int i = 0; i < columns; i++)
        data->columns[i] = create_column(STRING_TYPE, rows);
    file = fopen(filename,"r");
    int i = 0;
    if (file != NULL)
    {
        while (fgets(str, LEN, file) && (i < rows))
        {
            int j = 0;
            char* tok = strtok(str, delim);
            for (j = 0; j<columns && tok != NULL; j++)
            {

                //printf("i: %d, j: %d, %s", i, j ,tok);
                set_str(data->columns[j], tok, i);
                tok = strtok(NULL, delim);
            }
            i++;
        }
    }
    else
    {
        fprintf(stderr, "File not found.");
    }
    return data;
}

//int main()
//{
//	table* test = read_csv(3, 3, "data/1.csv", ",\n");
//	print_column(test->columns[0]);
//	print_column(test->columns[1]);
//	print_column(test->columns[2]);
//	return 0;
//}



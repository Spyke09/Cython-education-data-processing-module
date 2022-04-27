#include "table.h"

void read_csv(int rows, int columns, char* filename, char* delim)
{
   char str[LEN];
   FILE *file;
   char ***data;
   data = (char***)malloc(rows*sizeof(char**));
   for (int i = 0; i < rows; i++)
   {
       data[i] = (char**)malloc(columns*sizeof(char*));
    }
   file = fopen(filename,"r");
   int i = 0;
   if (file != NULL)
   {
        while (fgets(str, LEN, file) && (i < rows))
        {
            int j = 0;
            char* tok = strtok(str, delim);
            for (j = 0; j<columns; j++)
            {
                while (tok != NULL)
                {
                    data[i][j] = tok;
                    printf("%s\t", data[i][j]);
                    tok = strtok(NULL, delim);
                }
            }
            printf("\n");
            i++;
        }
    }
    else
    {
        fprintf(stderr, "File not found.");
    }
}

int main()
{
	read_csv(5010, 5, "data/orders.csv", ",");
	return 0;
}



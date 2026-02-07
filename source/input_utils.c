#include <input_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void flush_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void wait_for_enter_key(const char *message)
{
    printf("\nPress Enter to %s ...", message);
    getchar(); // now this truly waits for Enter
}

static int find_ohe_index(const int *ohe_indexes, int ohe_count, int col)
{
    for (int i = 0; i < ohe_count; i++)
        if (ohe_indexes[i] == col)
            return i;
    return -1;
}

static int find_category_index(const OHE_Column *col, const char *value)
{
    for (int i = 0; i < col->count; i++)
    {
        // printf("Compare: %s , %s ;\n", col->ohe_values[i], value);
        if (strcmp(col->ohe_values[i], value) == 0)
        {
            // printf("Equal: %s , %s ;\n", col->ohe_values[i], value);
            return i;
        }
    }
    return -1;
}

Dataset *read_from_csv_to_dataset(
    const char *csv_file,
    int *ohe_indexes,
    int ohe_count,
    const OHE_Column *ohe_maps,
    char ***columnNames)
{
    return read_from_stream_to_dataset(fopen(csv_file, "r"), ohe_indexes, ohe_count, ohe_maps, columnNames);
}

// custom features here
Dataset *read_from_stream_to_dataset(
    FILE *stream,
    int *ohe_indexes,
    int ohe_count,
    const OHE_Column *ohe_maps,
    char ***columnNames)
{
    FILE *fp = stream;
    if (!fp)
    {
        fprintf(stderr, "Error: cannot open file\n");
        return NULL;
    }

    char line[4096];
    fgets(line, sizeof(line), fp);

    int col_count = 1;
    for (char *p = line; *p; p++)
        if (*p == ',')
            col_count++;

    int row_count = 0;
    while (fgets(line, sizeof(line), fp))
        row_count++;

    rewind(fp);

    int final_cols = 0;

    for (int col = 0; col < col_count; col++)
    {
        int ohe_id = find_ohe_index(ohe_indexes, ohe_count, col);
        if (ohe_id >= 0)
        {
            final_cols += ohe_maps[ohe_id].count;
        }
        else
        {
            final_cols += 1;
        }
    }

    if (columnNames)
    {
        char **str_array = malloc(sizeof(char *) * final_cols);
        for (int i = 0; i < final_cols; i++)
        {
            str_array[i] = malloc(sizeof(char) * 512);
        }

        fgets(line, sizeof(line), fp);

        // remove \n from str
        line[strlen(line) - 1] = '\0';

        int col = 0;
        int out_col = 0;

        char *token = strtok(line, ",");
        while (token)
        {
            int ohe_id = find_ohe_index(ohe_indexes, ohe_count, col);

            if (ohe_id >= 0)
            {
                for (int k = 0; k < ohe_maps[ohe_id].count; k++)
                    strcpy(str_array[out_col + k], ohe_maps[ohe_id].ohe_values[k]);

                out_col += ohe_maps[ohe_id].count;
            }
            else
            {
                strcpy(str_array[out_col], token);
                out_col++;
            }

            token = strtok(NULL, ",");
            col++;
        }

        *columnNames = str_array;
    }

    Dataset *d = dataset_new(row_count, final_cols);

    int row = 0;
    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = '\0';

        int col = 0;
        int out_col = 0;
        int bad_row = 0;

        // copy from line to count tokens
        char temp[4096];
        strcpy(temp, line);

        // counting the tokens
        int token_count = 0;
        char *t = strtok(temp, ",");
        while (t)
        {
            token_count++;
            t = strtok(NULL, ",");
        }

        // skip if there is no prper tokens
        if (token_count != col_count)
        {
            continue;
        }

        // start parsing the row
        char *token = strtok(line, ",");
        while (token)
        {
            int ohe_id = find_ohe_index(ohe_indexes, ohe_count, col);

            if (ohe_id >= 0)
            {
                int cat_index = find_category_index(&ohe_maps[ohe_id], token);
                if (cat_index < 0)
                {
                    bad_row = 1;
                    break;
                }

                for (int k = 0; k < ohe_maps[ohe_id].count; k++)
                    d->data[row][out_col + k] = (k == cat_index) ? 1.0 : 0.0;

                out_col += ohe_maps[ohe_id].count;
            }
            else
            {
                if (token[0] == '\0')
                {
                    bad_row = 1;
                    break;
                }

                d->data[row][out_col] = atof(token);
                out_col++;
            }

            token = strtok(NULL, ",");
            col++;
        }

        if (!bad_row)
            row++;
    }

    d->max_rows = row;

    fclose(fp);
    return d;
}

// START - key handling - AI written

#ifdef _WIN32
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int linux_kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}
#endif

//
int cross_platform_kbhit(void)
{
#ifdef _WIN32
    return _kbhit();
#else
    return linux_kbhit();
#endif
}
char get_key_pressed(void)
{
#ifdef _WIN32
    return _getch();
#else
    return getchar();
#endif
}

// END - key handling - AI written

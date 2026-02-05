#include<input_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// پیدا کردن ستون های دسته بندی
static int find_ohe_index(const int* ohe_indexes, int ohe_count, int col) {
    for (int i = 0; i < ohe_count; i++)
        if (ohe_indexes[i] == col)
            return i;
    return -1;
}

// پیدا کردن شماره ستون برای ۱ کردن آن مقدار در دیتاست
static int find_category_index(const OHE_Column* col, const char* value) {
    for (int i = 0; i < col->count; i++) {
        if (strcmp(col->ohe_values[i], value) == 0)
            return i;
    }
    return -1;
}


Dataset* read_from_csv_to_dataset(
    const char* csv_file,
    int* ohe_indexes,
    int ohe_count,
    const OHE_Column* ohe_maps
) {
    return read_from_stream_to_dataset(fopen(csv_file, "r"), ohe_indexes, ohe_count, ohe_maps);
}


// custom features here
Dataset* read_from_stream_to_dataset(
    FILE* stream,
    int* ohe_indexes,
    int ohe_count,
    const OHE_Column* ohe_maps
) {
    FILE* fp = stream;
    if (!fp) {
        fprintf(stderr, "Error: cannot open file\n");
        return NULL;
    }

    char line[4096];
    fgets(line, sizeof(line), fp);

    int col_count = 1;
    for (char* p = line; *p; p++)
        if (*p == ',') col_count++;

    int row_count = 0;
    while (fgets(line, sizeof(line), fp))
        row_count++;

    rewind(fp);
    fgets(line, sizeof(line), fp); // پریدن از هدر

    int final_cols = 0;

    for (int col = 0; col < col_count; col++) {
        int ohe_id = find_ohe_index(ohe_indexes, ohe_count, col);
        if (ohe_id >= 0) {
            final_cols += ohe_maps[ohe_id].count;
        } else {
            final_cols += 1;
        }
    }

    Dataset* d = dataset_new(row_count, final_cols);

    int row = 0;
    while (fgets(line, sizeof(line), fp)) {
        int col = 0;
        int out_col = 0;

        char* token = strtok(line, ",");
        while (token) {
            int ohe_id = find_ohe_index(ohe_indexes, ohe_count, col);

            if (ohe_id >= 0) {
                // OHE column
                int cat_index = find_category_index(&ohe_maps[ohe_id], token);

                for (int k = 0; k < ohe_maps[ohe_id].count; k++)
                    d->data[row][out_col + k] = (k == cat_index) ? 1.0 : 0.0;

                out_col += ohe_maps[ohe_id].count;
            } else {
                if (token[0] == '\0')
                {
                    row--;
                    break;
                }
                
                // numeric column
                d->data[row][out_col] = atof(token);
                out_col++;
            }

            token = strtok(NULL, ",");
            col++;
        }

        row++;
    }

    d->max_rows = row;

    fclose(fp);
    return d;
}

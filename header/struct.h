#ifndef STRUCT_H
#define STRUCT_H


// dataset->data[row][column]
typedef struct
{
    int max_rows;
    int max_cols;
    double **data;
} Dataset;

typedef struct
{
    double learning_rate;
    int epochs;
    double *weights_out;
    double bias_out;
} Weights;

Weights* weights_new(double learning_rate, int epochs, int count);
void weights_free(Weights *weights);

Dataset* dataset_new(int max_rows, int max_cols);
void dataset_free(Dataset *dataset);
void dataset_copy_row(Dataset *src, Dataset *dst, unsigned fromIndexSrc, unsigned toIndexDst);

#endif
#ifndef STRUCT_H
#define STRUCT_H

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
    int bias_out;
} Weights;

Weights weights_new(double learning_rate, int epochs, int feature_count);
void weights_free(Weights *weights);

Dataset dataset_new_raw(int max_rows, int max_cols);
void dataset_free(Dataset *dataset);


#endif
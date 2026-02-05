#ifndef STRUCT_H
#define STRUCT_H



// dataset->data[row][column]
typedef struct
{
    int max_rows;
    int max_cols;
    double **data;
    double *mean_values; // میانگین
    double *median_values; // میانه
    double *mode_values;
    double *max_values;
    double *min_values;
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

Dataset dataset_new(int max_rows, int max_cols);
void dataset_free(Dataset *dataset);

#endif
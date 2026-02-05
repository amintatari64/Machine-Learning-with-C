#include <struct.h>
#include <stdlib.h>

Weights weights_new(double learning_rate, int epochs, int feature_count)
{
    double *weights = (double *)malloc(sizeof(double) * feature_count);
    return (Weights){
        .learning_rate = learning_rate,
        .epochs = epochs,
        .weights_out = weights};
}
void weights_free(Weights *weights)
{
    free(weights->weights_out);
}

// dataset->data[row][column]
Dataset dataset_new(int max_rows, int max_cols)
{
    double **data = malloc(sizeof(double *) * max_rows);
    for (int i = 0; i < max_rows; i++)
    {
        data[i] = malloc(sizeof(double) * max_cols);
    }
    Dataset d = (Dataset){.data = data, .max_rows = max_rows, .max_cols = max_cols};
    d.median_values = malloc(sizeof(double) * max_cols); // میانه
    d.mean_values = malloc(sizeof(double) * max_cols); // میانگین
    d.max_values = malloc(sizeof(double) * max_cols); // ماکزیمم
    d.min_values = malloc(sizeof(double) * max_cols); // مینیمم
    return d;
}
void dataset_free(Dataset *dataset)
{
    for (int i = 0; i < dataset->max_rows; i++)
    {
        free(dataset->data[i]);
    }
    free(dataset->data);
}
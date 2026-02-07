#include <struct.h>
#include <stdlib.h>
#include <assert.h>

Weights *weights_new(double learning_rate, int epochs, int count)
{
    double *weights = (double *)malloc(sizeof(double) * count);
    Weights *w = malloc(sizeof(Weights));
    *w = (Weights){
        .learning_rate = learning_rate,
        .epochs = epochs,
        .weights_out = weights};
    return w;
}
void weights_free(Weights *weights)
{
    free(weights->weights_out);
    free(weights);
}

// dataset->data[row][column]
Dataset *dataset_new(int max_rows, int max_cols)
{
    double **data = malloc(sizeof(double *) * max_rows);
    for (int i = 0; i < max_rows; i++)
    {
        data[i] = malloc(sizeof(double) * max_cols);
    }
    Dataset *d = malloc(sizeof(Dataset));
    *d = (Dataset){.data = data, .max_rows = max_rows, .max_cols = max_cols};
    return d;
}
void dataset_free(Dataset *dataset)
{
    for (int i = 0; i < dataset->max_rows; i++)
    {
        free(dataset->data[i]);
    }
    free(dataset->data);
    free(dataset);
}

void dataset_copy_row(Dataset *src, Dataset *dst, unsigned fromIndexSrc, unsigned toIndexDst) {
    assert(src->max_cols <= dst->max_cols);
    
    for (int i = 0; i < src->max_cols; i++)
    {
        dst->data[toIndexDst][i] = src->data[fromIndexSrc][i]; 
    }
}
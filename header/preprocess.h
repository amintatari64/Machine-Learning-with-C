#include <struct.h>

#ifndef PREPROCESS_H
#define PREPROCESS_H

#define NORM_MIN 0.0
#define NORM_MAX 1.0

// for train data set only : writes the normalization ratios and normalization biases on the arrays for only features, not target.
Dataset *dataset_normalize_min_max(Dataset *not_normalized_ds, double *normalization_ratios_out, double *normalization_biases_out, int price_index);

// void dataset_update_parameters(Dataset *dataset);

// Dataset *train;
// Dataset *test;
// dataset_split_train_test(input, &test, &train, 0.8);
void dataset_split_train_test(Dataset *dataset, Dataset **train_data_set, Dataset **test_data_set, float train_ratio);

// size: dataset->max_rows - 1
Dataset *convert_to_normalized(Dataset *not_normalized_ds, double *normalization_ratios, double *normalization_biases);

Dataset *dataset_add_custom_feature(
    const Dataset *src,
    double (*formula)(double *row));

void analyze_points(const double *x, const double *y, int n, double *slope, double *intercept, double *correlation);

#endif
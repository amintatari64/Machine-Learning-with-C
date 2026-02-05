#include <struct.h>

#ifndef PREPROCESS_H
#define PREPROCESS_H

// #define min_normalize 0
// #define max_normalize 1

// @amintatari64
// for train data set only : writes the normalization ratios and normalization biases on the arrays for only features, not target.
void dataset_normalize_min_max(Dataset* dataset, double* normalization_ratios_out, double* normalization_biases_out);

// void dataset_update_parameters(Dataset *dataset);

// @mhrohani1385

// Dataset *train;
// Dataset *test;
// dataset_split_train_test(input, &test, &train, 0.8);
void dataset_split_train_test(Dataset* dataset, Dataset** train_data_set, Dataset** test_data_set, float train_ratio);

// @amintatari64
// size: dataset->max_rows - 1
void convert_to_normalized(Dataset* dataset, double* normalization_ratios, double* normalization_biases);

#endif
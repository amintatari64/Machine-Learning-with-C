#include<struct.h>

#ifndef TRAIN_UTILS_H
#define TRAIN_UTILS_H

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init);

void calculate_model_performance_metrics(int *MSE, int *MAPE, Dataset *test_dataset, Weights *model);
void dataset_update_parameters(Dataset *dataset);

void dataset_split_train_test(Dataset* dataset, Dataset** train_data_set, Dataset** test_data_set, float train_ratio);

// for train data set only : writes the normalization ratios and normalization biases on the arrays for only features, not target.
void dataset_normalize_min_max(Dataset* dataset, double* normalization_ratios, double* normalization_biases);

void convert_to_normalized(Dataset* dataset, double* normalization_ratios, double* normalization_biases);

#endif
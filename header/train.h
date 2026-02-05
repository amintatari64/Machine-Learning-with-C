#include<struct.h>

#ifndef TRAIN_UTILS_H
#define TRAIN_UTILS_H

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init);

// @amintatari64
void calculate_model_performance_metrics(int *MSE, int *MAPE, Dataset *test_dataset_normalized, Weights *model);

#endif
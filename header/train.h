#include<struct.h>

#ifndef TRAIN_UTILS_H
#define TRAIN_UTILS_H

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init, int target_i);


void calculate_model_performance_metrics(
        double *MSE,
        double *MAPE,
        Dataset *test_dataset_normalized,
        Weights *model,
        int y_index
);

#endif
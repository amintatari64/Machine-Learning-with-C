#include <struct.h>

#ifndef TRAIN_UTILS_H
#define TRAIN_UTILS_H

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init, int target_i);

void predict_prices_from_dataset(
    const Dataset *test_dataset_normalized,
    const Weights *model,
    double *y_pred_prices);

void calculate_mse_mape(
    double *MSE,
    double *MAPE,
    const double *y_pred_prices,
    const double *y_true_prices,
    int size);

#endif
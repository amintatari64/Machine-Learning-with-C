#include <train.h>
#include <math.h>
#include <input_utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stddef.h>

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init, int target_i)
{
    const double learning_rate = weights_init->learning_rate;
    const int epochs = weights_init->epochs;

    const int max_rows = data->max_rows;
    const int max_cols = data->max_cols;

    double weights[max_cols];
    weights[target_i] = 0;

    // initialize
    for (int j = 0; j < max_cols; j++)
    {
        weights[j] = 0.0;
    }

    double bias = 0.0;
    volatile int cancel_training = 0;
    for (int epoch = 0; epoch < epochs; epoch++)
    {

        if (cross_platform_kbhit())
        {
            char c = get_key_pressed();
            if (c == 's' || c == 'S')
            {
                cancel_training = 1;
            }
        }
        if (cancel_training)
        {
            printf("\nTraining stopped by user\n");
            break;
        }

        double grad_weights[max_cols];
        for (int j = 0; j < max_cols; j++)
        {
            grad_weights[j] = 0.0;
        }
        double grad_bias = 0.0;

        for (int i = 0; i < max_rows; i++)
        {

            double y_hat = bias;

            for (int j = 0; j < max_cols; j++)
            {
                if (j == target_i)
                    continue;
                y_hat += weights[j] * data->data[i][j];
            }

            double error = y_hat - data->data[i][target_i];

            for (int j = 0; j < max_cols; j++)
            {
                if (j == target_i)
                    continue;
                grad_weights[j] += error * data->data[i][j];
            }

            grad_bias += error;
        }

        for (int j = 0; j < max_cols; j++)
        {
            if (j == target_i)
                continue;
            weights[j] -= learning_rate * (grad_weights[j] / max_rows);
        }
        bias -= learning_rate * (grad_bias / max_rows);

        // Report
        if (epoch % 100 == 0)
        {
            double mse = 0;
            for (int i = 0; i < max_rows; i++)
            {
                double pred = bias;
                for (int j = 0; j < max_cols; j++)
                {
                    if (j == target_i)
                        continue;
                    pred += weights[j] * data->data[i][j];
                }
                double diff = pred - data->data[i][target_i];
                mse += diff * diff;
            }
            mse /= max_rows;
            printf("Epoch %d: MSE = %.4f\n", epoch, mse);
        }
    }

    // Output
    // printf("\nFinal Model:\n");
    // printf("Bias: %.4f\n", bias);
    weights_init->bias_out = bias;

    for (int j = 0; j < max_cols; j++)
    {
        // printf("Weight[%d]: %.4f%s\n", j, weights[j],
        //    (j == target_i ? " (ignored target)" : ""));
        weights_init->weights_out[j] = weights[j];
    }
}


void predict_prices_from_dataset(
        const Dataset *test_dataset_normalized,
        const Weights *model,
        double *y_pred_prices
) {
    // Validate pointers
    if (!test_dataset_normalized || !model || !model->weights_out || !y_pred_prices) {
        return;
    }

    const int rows = test_dataset_normalized->max_rows;
    const int cols = test_dataset_normalized->max_cols;

    // Validate dataset dimensions and y index
    if (rows <= 0 || cols <= 1 ) {
        return;
    }

    // Assumption: model->weights_out has exactly num_features elements
    for (int i = 0; i < rows; i++) {
        // Start with output bias
        double y_pred = (double)model->bias_out;

        int w_idx = 0;
        for (int j = 0; j < cols; j++) {

            // Linear model: y = w*x + b
            y_pred += test_dataset_normalized->data[i][j] * model->weights_out[w_idx];
            w_idx++;
        }

        // Store predicted price
        y_pred_prices[i] = y_pred;
    }
}


void calculate_mse_mape(
        double *MSE,
        double *MAPE,
        const double *y_pred_prices,
        const double *y_true_prices,
        int size
) {
    // if (!MSE || !MAPE || !y_pred_prices || !y_true_prices || size <= 0) {
    //     return;
    // }

    double sum_sq_err = 0.0;
    double sum_ape = 0.0;
    int mape_valid_count = 0;

    for (int i = 0; i < size; i++) {
        const double y_true = y_true_prices[i];
        const double y_pred = y_pred_prices[i];

        const double err = y_true - y_pred;
        sum_sq_err += err * err;

        if (fabs(y_true) > 1e-12)
        {
            sum_ape += fabs(err / y_true);
            mape_valid_count++;
        }
    }

    const double mse = sum_sq_err / (double)size;

    double mape = 0.0;
    if (mape_valid_count > 0) {
        mape = (sum_ape / (double)mape_valid_count) * 100.0;
    }

    *MSE = mse;
    *MAPE = mape;
}





//double mse_real = ((double)MSE_int) / 10000.0;

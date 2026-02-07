#include <train.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init, int target_i)
{
    const double learning_rate = weights_init->learning_rate;
    const int epochs = weights_init->epochs;

    const int max_rows = data->max_rows;
    const int max_cols = data->max_cols;

    double weights[max_cols];

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



void calculate_model_performance_metrics(
    double *MSE,
    double *MAPE,
    Dataset *test_dataset_normalized,
    Weights *model,
    int y_index)
{
    if (!MSE || !MAPE || !test_dataset_normalized || !model || !model->weights_out)
    {
        return;
    }

    int rows = test_dataset_normalized->max_rows;
    int cols = test_dataset_normalized->max_cols;

    if (rows <= 0 || cols <= 1 || y_index < 0 || y_index >= cols)
    {
        *MSE = 0;
        *MAPE = 0;
        return;
    }

    double sum_sq_err = 0.0;
    double sum_ape = 0.0;
    int mape_valid_count = 0;

    for (int i = 0; i < rows; i++)
    {
        double y_true = test_dataset_normalized->data[i][y_index];

        double y_pred = (double)model->bias_out;
        int w_idx = 0;

        for (int j = 0; j < cols; j++)
        {
            if (j == y_index)
                continue;

            y_pred += test_dataset_normalized->data[i][j] * model->weights_out[w_idx];
            w_idx++;
        }

        double err = y_true - y_pred;
        sum_sq_err += err * err;

        if (fabs(y_true) > 1e-12)
        {
            sum_ape += fabs(err / y_true);
            mape_valid_count++;
        }
    }

    double mse = sum_sq_err / (double)rows;
    double mape = 0.0;
    if (mape_valid_count > 0)
    {
        mape = (sum_ape / (double)mape_valid_count) * 100.0;
    }

    *MSE = mse;
    *MAPE = mape;
}

// double mse_real = ((double)MSE_int) / 10000.0;

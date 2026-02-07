#include<train.h>
#include <math.h>
#include <stddef.h>


void linear_regression_calculate_weights(Dataset *data, Weights *weights_init)
{

    const int learning_rate = weights_init->learning_rate, epochs = weights_init->epochs;

    const int max_rows = data->max_rows;
    const int max_cols = data->max_cols;
    const int num_features = max_cols - 1; // sotoon akhar target(price)(y)

    // 10 ha ro be 9 taghir bedid agar encode nakardid
    double weights[num_features];
    // initialize::
    for (int i = 0; i < num_features; i++) {
        weights[i] = 0;
    }

    double bias = 0.0;

    // Training loop

    for (int epoch = 0; epoch < epochs; epoch++) {
        double grad_weights[num_features];
        // initialize::
        for (int i = 0; i < num_features; i++) {
            grad_weights[i] = 0.0;
        }

        double grad_bias = 0.0;

        // Calculate gradients
        for (int i = 0; i < max_rows; i++) {
            // Predict
            double y_hat = bias;
            for (int j = 0; j < num_features; j++) {
                y_hat += weights[j] * data->data[i][j];
            }
            double error = y_hat - data->data[i][num_features];

            for (int j = 0; j < num_features; j++) {
                grad_weights[j] += error * data->data[i][j];
            }
            grad_bias += error;
        }

        // Update weights and bias
        for (int j = 0; j < num_features; j++) {
            weights[j] -= learning_rate * (grad_weights[j] / max_rows);
        }
        bias -= learning_rate * (grad_bias / max_rows);

        // har 100 epoch vaziat ro gozaresh bede
        if (epoch % 100 == 0) {
            double mse = 0;
            for (int i = 0; i < max_rows; i++) {
                double pred = bias;
                for (int j = 0; j < num_features; j++) {
                    pred += weights[j] * data->data[i][j];
                }
                double diff = pred - data->data[i][num_features];
                mse += diff * diff;
            }
            mse /= max_rows;
            printf("Epoch %d: MSE = %.4f\n", epoch, mse);
        }
    }

    // Print final model
    printf("\nFinal Model:\n");
    printf("Bias: %.4f\n", bias);
    weights_init->bias_out = bias;
    for (int j = 0; j < num_features; j++) {
        printf("Weight[%d]: %.4f\n", j, weights[j]);
        weights_init->weights_out[j] = weights[j];
    }
}


/*
    Returns: number of predicted values (equals dataset row count)

    Inputs:
      - test_dataset_normalized
      - model: trained model weights and bias
      - y_pred_prices: pre-allocated array to store predicted prices

    How to use:
        y_pred_prices = malloc(sizeof(double) * test_dataset_normalized->max_rows);
        int n = predict_prices_from_dataset(ds, model, y_pred_prices, y_index);
 */
int predict_prices_from_dataset(
        const Dataset *test_dataset_normalized,
        const Weights *model,
        double *y_pred_prices,
) {
    // Validate pointers
    if (!test_dataset_normalized || !model || !model->weights_out || !y_pred_prices) {
        return 0;
    }

    const int rows = test_dataset_normalized->max_rows;
    const int cols = test_dataset_normalized->max_cols;

    // Validate dataset dimensions and y index
    if (rows <= 0 || cols <= 1 ) {
        return 0;
    }


    const int num_features = cols;

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

    // Return number of predictions
    return rows;
}


void calculate_model_performance_metrics(
        int *MSE,
        int *MAPE,
        const double *y_pred_prices,
        const double *y_true_prices,
        int size
) {
    if (!MSE || !MAPE || !y_pred_prices || !y_true_prices || size <= 0) {
        return;
    }

    double sum_sq_err = 0.0;
    double sum_ape = 0.0;
    int mape_valid_count = 0;

    for (int i = 0; i < size; i++) {
        const double y_true = y_true_prices[i];
        const double y_pred = y_pred_prices[i];

        const double err = y_true - y_pred;
        sum_sq_err += err * err;

        // avoid division by zero (or near zero)
        if (fabs(y_true) > 1e-12) {
            sum_ape += fabs(err / y_true);
            mape_valid_count++;
        }
    }

    const double mse = sum_sq_err / (double)size;

    double mape = 0.0;
    if (mape_valid_count > 0) {
        mape = (sum_ape / (double)mape_valid_count) * 100.0;
    }

    // keep your previous scaling behavior for int output
    const double MSE_SCALE = 10000.0;

    *MSE = (int)llround(mse * MSE_SCALE);
    *MAPE = (int)llround(mape);
}





//double mse_real = ((double)MSE_int) / 10000.0;

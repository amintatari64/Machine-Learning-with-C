#include<train_utils.h>

void dataset_split_train_test(Dataset* dataset, Dataset** train_data_set, Dataset** test_data_set, float train_ratio) {
    
}

// for train data set only : writes the normalization ratios and normalization biases on the arrays for only features, not target.
void dataset_normalize_min_max(Dataset* dataset, double* normalization_ratios, double* normalization_biases) {
    
}

void convert_to_normalized(Dataset* dataset, double* normalization_ratios, double* normalization_biases) {
    
}

void dataset_update_parameters(Dataset* dataset) {
    
}

void linear_regression_calculate_weights(Dataset *data, Weights *weights_init)
{

    const int learning_rate = weights_init->learning_rate , epochs = weights_init->epochs;

    const int max_rows = data->max_rows;
    const int max_cols = data->max_cols;
    const int num_features = max_cols - 1; // sotoon akhar target(price)(y)

    // 10 ha ro be 9 taghir bedid agar encode nakardid
    double weights[num_features];
    // initialize::
    for (int i = 0; i < num_features; i++)
    {
        weights[i] = 0;
    }

    double bias = 0.0;

    // Training loop

    for (int epoch = 0; epoch < epochs; epoch++)
    {
        double grad_weights[num_features];
        // initialize::
        for (int i = 0; i < num_features; i++)
        {
            grad_weights[i] = 0.0;
        }

        double grad_bias = 0.0;

        // Calculate gradients
        for (int i = 0; i < max_rows; i++)
        {
            // Predict
            double y_hat = bias;
            for (int j = 0; j < num_features; j++)
            {
                y_hat += weights[j] * data->data[i][j];
            }
            double error = y_hat - data->data[i][num_features];

            for (int j = 0; j < num_features; j++)
            {
                grad_weights[j] += error * data->data[i][j];
            }
            grad_bias += error;
        }

        // Update weights and bias
        for (int j = 0; j < num_features; j++)
        {
            weights[j] -= learning_rate * (grad_weights[j] / max_rows);
        }
        bias -= learning_rate * (grad_bias / max_rows);

        // har 100 epoch vaziat ro gozaresh bede
        if (epoch % 100 == 0)
        {
            double mse = 0;
            for (int i = 0; i < max_rows; i++)
            {
                double pred = bias;
                for (int j = 0; j < num_features; j++)
                {
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
    for (int j = 0; j < num_features; j++)
    {
        printf("Weight[%d]: %.4f\n", j, weights[j]);
        weights_init->weights_out[j] = weights[j];
    }
}
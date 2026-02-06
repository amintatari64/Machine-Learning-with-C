#include<preprocess.h>

void dataset_normalize_min_max(
        Dataset* dataset,
        double* normalization_ratios,
        double* normalization_biases
) {
    if (!dataset || !dataset->data || dataset->max_rows <= 0 || dataset->max_cols <= 1) {
        return;
    }

    const int max_rows = dataset->max_rows;
    const int num_features = dataset->max_cols - 1;

    for (int col = 0; col < num_features; col++) {
        double min_val = dataset->data[0][col];
        double max_val = dataset->data[0][col];

        for (int row = 1; row < max_rows; row++) {
            const double v = dataset->data[row][col];
            if (v < min_val) min_val = v;
            if (v > max_val) max_val = v;
        }

        if (max_val == min_val) {
            normalization_ratios[col] = 0.0;
            normalization_biases[col] = NORM_MIN;

            for (int row = 0; row < max_rows; row++) {
                dataset->data[row][col] = NORM_MIN;
            }
            continue;
        }

        const double ratio =
                (NORM_MAX - NORM_MIN) / (max_val - min_val);
        const double bias =
                NORM_MIN - min_val * ratio;

        normalization_ratios[col] = ratio;
        normalization_biases[col] = bias;

        for (int row = 0; row < max_rows; row++) {
            dataset->data[row][col] =
                    dataset->data[row][col] * ratio + bias;
        }
    }
}

void convert_to_normalized(
        Dataset* dataset,
        double* normalization_ratios,
        double* normalization_biases
) {
    if (!dataset || !dataset->data || dataset->max_rows <= 0 || dataset->max_cols <= 1) {
        return;
    }

    const int max_rows = dataset->max_rows;
    const int num_features = dataset->max_cols - 1;

    for (int col = 0; col < num_features; col++) {
        const double ratio = normalization_ratios[col];
        const double bias = normalization_biases[col];

        for (int row = 0; row < max_rows; row++) {
            dataset->data[row][col] =
                    dataset->data[row][col] * ratio + bias;
        }
    }
}

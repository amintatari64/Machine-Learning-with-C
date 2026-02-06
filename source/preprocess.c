#include <preprocess.h>
#include <stdlib.h>
#include <time.h>

void analyze_points(const double *x, const double *y, int n, double *slope, double *intercept, double *correlation)
{
    double sum_x = 0, sum_y = 0;
    double sum_xy = 0, sum_x2 = 0, sum_y2 = 0;

    for (int i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_xy += x[i] * y[i];
        sum_x2 += x[i] * x[i];
        sum_y2 += y[i] * y[i];
    }

    double numerator = n * sum_xy - sum_x * sum_y;
    double denom_y = n * sum_y2 - sum_y * sum_y;

    double denom_x = n * sum_x2 - sum_x * sum_x;

    if (denom_x == 0)
    {
        *slope = 0;
        *intercept = 0;
    }
    else
    {
        *slope = numerator / denom_x;
        *intercept = (sum_y - (*slope) * sum_x) / n;
    }

    double denominator = sqrt(denom_x * denom_y);

    *correlation = denominator == 0 ? 0 : numerator / denominator;
}

// free dataset if you want
void dataset_split_train_test(Dataset *dataset, Dataset **train_data_ptr, Dataset **test_data_ptr, float train_ratio)
{
    int c = dataset->max_rows;

    int train_c = c * train_ratio;
    int test_c = c - train_c;

    int column_count = dataset->max_cols;

    _Bool arrayIndexesForTrain[c];
    for (int i = 0; i < c; i++)
    {
        arrayIndexesForTrain[i] = 1;
    }

    int random_count = 0;
    srand(time(NULL));

    while (random_count < test_c)
    {
        int index = rand() % c;
        if (arrayIndexesForTrain[index])
        {
            arrayIndexesForTrain[index] = 0;
            random_count++;
        }
    }

    Dataset *train = dataset_new(train_c, column_count);
    Dataset *test = dataset_new(test_c, column_count);

    int train_i = 0;
    int test_i = 0;

    for (int i = 0; i < c; i++)
    {
        if (arrayIndexesForTrain[i])
        {
            dataset_copy_row(dataset, train, i, train_i++);
        } else {
            dataset_copy_row(dataset, test, i, test_i++);
        }
    }
}
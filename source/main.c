#include <stdio.h>
#include <plot_utils.h>
#include <input_utils.h>
#include <preprocess.h>
#include <stdlib.h>
#include <train.h>

Weights *make_model(Dataset *train_ds, int target_i, double learning_rate, int epochs)
{
    Weights *w = weights_new(learning_rate, epochs, train_ds->max_cols);
    linear_regression_calculate_weights(train_ds, w, target_i);
    return w;
}
void split_train_test(Dataset *main_ds, Dataset **train_ds, Dataset **test_ds, double *normalization_ratios, double *normalization_biases, int price_column_i)
{
    if (*train_ds)
        dataset_free(*train_ds);
    if (*test_ds)
        dataset_free(*test_ds);

    float train_ratio = -1;
    while (1 < train_ratio || train_ratio <= 0)
    {
        printf("Enter train ratio from 0.0 to 1.0 :: ");
        scanf("%f", &train_ratio);
        flush_stdin();
    }

    dataset_split_train_test(main_ds, train_ds, test_ds, train_ratio);
    printf("Dataset successfully loaded with %d train and %d test data records from all %d data record\n", (*train_ds)->max_rows, (*test_ds)->max_rows, main_ds->max_rows);

    // normailization
    Dataset *d = dataset_normalize_min_max(*train_ds, normalization_ratios, normalization_biases, price_column_i);
    dataset_free(*train_ds);
    *train_ds = d;
    printf("Train dataset successfully normalized");

    wait_for_enter_key("continue");
}
void caclulate_model(Dataset *train_ds, Weights **model_ptr, int price_column_i)
{
    if (train_ds)
    {
        if (*model_ptr)
        {
            weights_free(*model_ptr);
        }
        
        double lr;
        int ep;
        printf("Model calculation : \nEnter a learning rate : ");
        scanf("%lf", &lr);
        printf("Enter epoch count : ");
        scanf("%d", &ep);
        printf("\nModel calculation may take a long time. if you want to stop the process, press 'S' key on keyboard\n");
        flush_stdin();
        wait_for_enter_key("start calculation");

        *model_ptr = make_model(train_ds, price_column_i, lr, ep);
        for (int col_i = 0; col_i < train_ds->max_cols; col_i++)
        {
            if (col_i != price_column_i)
            {
                printf(" |%lf| ", (*model_ptr)->weights_out[col_i]);
            }
            else
            {
                printf(" |--| ");
            }
        }
        printf(" + %lf", (*model_ptr)->bias_out);
    }
    else
    {
        printf("Train dataset not loaded yet. Please load the dataset with (2) first.\n");
        flush_stdin();
    }
    wait_for_enter_key("continue");
}

void check_model_performance(Dataset *test_ds, Weights *weights_model, double *normalization_ratios, double *normalization_biases, int price_column_i)
{
    if (weights_model)
    {
        Dataset *test_ds_norm = convert_to_normalized(test_ds, normalization_ratios, normalization_biases);
        double MSE, MAPE;
        calculate_model_performance_metrics(&MSE, &MAPE, test_ds_norm, weights_model, price_column_i);
        printf("MSE : %lf\nMAPE : %lf\n", MSE, MAPE);
    }
    else
    {
        printf("Weights model not calculated yet. Please calculate the model with (3) first.\n");
    }
    flush_stdin();
    wait_for_enter_key("continue");
}

int command_loop()
{
    const int price_column_i = 8;

    char **column_names;

    Dataset *main_ds = read_from_csv_to_dataset(
        "housingprices.csv",
        (int[]){9},
        1,
        (OHE_Column[]){
            (OHE_Column){
                .ohe_values = (char *[]){"<1H OCEAN", "INLAND", "NEAR BAY", "NEAR OCEAN", "ISLAND"},
                .count = 5}},
        &column_names);

    Dataset *train_ds = NULL, *test_ds = NULL;
    Weights *weights_model = NULL;

    double normalization_ratios[main_ds->max_cols], normalization_biases[main_ds->max_cols];

    // input command loop
    _Bool continue_run = 1;
    while (continue_run)
    {
        printf("\nEnter command: \n"
               "1. Plot points to check features versus the target and their correlation in dataset\n"
               "2. Make and load the train and test dataset\n"
               "3. Find weights and produce the model with gradian decent - linear regression\n"
               "4. Check the model performance and percision\n"
               "5. Exit\n\n");

        printf(":: ");

        int command_code = getchar() - '0';

        switch (command_code)
        {

        case 1:
            plot_price_versus_features(main_ds, column_names, price_column_i);
            break;
        case 2:
            split_train_test(main_ds, &train_ds, &test_ds, normalization_ratios, normalization_biases, price_column_i);
            break;
        case 3:
            caclulate_model(train_ds, &weights_model, price_column_i);
            break;
        case 4:
            check_model_performance(test_ds, weights_model, normalization_ratios, normalization_biases, price_column_i);
            break;
        case 5:
            continue_run = 0;
            break;

        default:
            printf("Enter a valid command\n");
            flush_stdin();
            wait_for_enter_key("continue");
            break;
        }
    }

    dataset_free(main_ds);
    if (test_ds)
        dataset_free(test_ds);
    if (train_ds)
        dataset_free(train_ds);
    if (weights_model)
        weights_free(weights_model);

    return 0;
}

int main()
{
    command_loop();
    // double x[5] = {1, 2, 3, 4, 5}, y[5] = {1,2.2,2.7,2.8,5.2};
    // plot_actual_vs_predicted(x, y, 5, 0.5, 0.5);
}
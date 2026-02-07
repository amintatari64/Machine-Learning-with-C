#include <stdio.h>
#include <plot_utils.h>
#include <input_utils.h>
#include <preprocess.h>
#include <stdlib.h>
#include <train.h>
#include <string.h>

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
void caclulate_model(Dataset *train_ds, Weights **model_ptr, char **column_names, int price_column_i)
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
        flush_stdin();
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
                printf(" |%s: %lf| ", column_names[col_i], (*model_ptr)->weights_out[col_i]);
            }
            else
            {
                printf(" |price: --| ");
            }
        }
        printf(" + bias : %lf", (*model_ptr)->bias_out);
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
        double y_predicts[test_ds->max_rows];
        double y_reals[test_ds->max_rows];

        Dataset *test_ds_norm = convert_to_normalized(test_ds, normalization_ratios, normalization_biases);
        predict_prices_from_dataset(test_ds_norm, weights_model, y_predicts);
        for (int r = 0; r < test_ds_norm->max_rows; r++)
        {
            y_reals[r] = test_ds_norm->data[r][price_column_i];
        }

        double MSE, MAPE;
        calculate_mse_mape(&MSE, &MAPE, y_predicts, y_reals, test_ds_norm->max_rows);
        printf("MSE : %lf\nMAPE : %lf\n", MSE, MAPE);

        plot_actual_vs_predicted(y_reals, y_predicts, test_ds_norm->max_rows, MSE, MAPE);
    }
    else
    {
        printf("Weights model not calculated yet. Please calculate the model with (3) first.\n");
        flush_stdin();
    }

    wait_for_enter_key("continue");
}

void custom_home_price_prediction(
    Weights *weights_model,
    double *normalization_ratios,
    double *normalization_biases,
    char **column_names,
    int data_count,
    int price_column_i)
{
    if (!weights_model)
    {
        printf("Weights model not calculated yet. Please calculate the model with (3) first.\n");
        flush_stdin();
        wait_for_enter_key("continue");
        return;
    }

    printf("\nCustom Home Price Prediction\n");
    printf("Enter values for each feature:\n\n");

    double input_row[data_count];

    for (int i = 0; i < data_count; i++)
    {
        if (i == price_column_i)
        {
            input_row[price_column_i] = 0;
            continue;
        }

        printf("%s : ", column_names[i]);
        scanf("%lf", &input_row[i]);
        flush_stdin();
    }

    // normalize
    double normalized_row[data_count];
    for (int i = 0; i < data_count; i++)
    {
        normalized_row[i] =
            input_row[i] * normalization_ratios[i] + normalization_biases[i];
    }

    double predicted = weights_model->bias_out;
    for (int i = 0; i < data_count; i++)
    {
        if (i != price_column_i)
        {
            predicted += normalized_row[i] * weights_model->weights_out[i];
        }
    }

    printf("\nPredicted price = %lf\n", predicted);

    wait_for_enter_key("continue");
}

char **column_names_add_custom(char **src, int old_count)
{
    char **dst = malloc(sizeof(char *) * (old_count + 1));
    if (!dst)
        return NULL;

    for (int i = 0; i <= old_count; i++)
    {
        dst[i] = malloc(sizeof(char) * 512);
        if (i == old_count)
        {
            strcpy(dst[i], "custom feature");
        }
        else
        {
            strcpy(dst[i], src[i]);
        }
    }

    return dst;
}

double custom_formula(double *row)
{
    double median_income = row[7];
    double housing_median_age = row[2];
    double is_INLAND = row[10];

    // median_income ^ 2 + 0.5 * age - 2 * is_inland
    return (median_income * median_income) + 0.5 * housing_median_age - 2.0 * is_INLAND;
}

int command_loop()
{
    const int price_column_i = 8;

    char **column_names_org;
    char **column_names;

    _Bool custom_feature_added = 0;
    Dataset *original_ds = read_from_csv_to_dataset(
        "housingprices.csv",
        (int[]){9},
        1,
        (OHE_Column[]){
            (OHE_Column){
                .ohe_values = (char *[]){"<1H OCEAN", "INLAND", "NEAR BAY", "NEAR OCEAN", "ISLAND"},
                .count = 5}},
        &column_names_org);

    column_names = column_names_org;

    Dataset *main_ds = original_ds;

    Dataset *cf_added_ds = dataset_add_custom_feature(original_ds, custom_formula);

    char **column_names_cf = column_names_add_custom(column_names_org, original_ds->max_cols);

    Dataset *train_ds = NULL, *test_ds = NULL;
    Weights *weights_model = NULL;

    double normalization_ratios[cf_added_ds->max_cols], normalization_biases[cf_added_ds->max_cols];

    // input command loop
    _Bool continue_run = 1;
    while (continue_run)
    {
        printf("\nEnter command: \n"
               "1. Plot points to check features versus the target and their correlation in dataset\n"
               "2. Make and load the train and test dataset\n"
               "3. Find weights and produce the model with gradian decent - linear regression\n"
               "4. Check the model performance and percision\n"
               "5. Custom home price prediction\n"
               "6. Add\\Remove custom feature to main dataset (%c)\n"
               "7. Exit\n\n",
               custom_feature_added ? 'x' : ' ');

        printf(":: ");

        char input_cmd[10];
        scanf("%s", input_cmd);
        int command_code = strtol(input_cmd, NULL, 0);

        switch (command_code)
        {

        case 1:
            plot_price_versus_features(main_ds, column_names, price_column_i);
            break;
        case 2:
            split_train_test(main_ds, &train_ds, &test_ds, normalization_ratios, normalization_biases, price_column_i);
            break;
        case 3:
            caclulate_model(train_ds, &weights_model, column_names, price_column_i);
            break;
        case 4:
            check_model_performance(test_ds, weights_model, normalization_ratios, normalization_biases, price_column_i);
            break;
        case 5:
            custom_home_price_prediction(
                weights_model,
                normalization_ratios,
                normalization_biases,
                column_names,
                main_ds->max_cols,
                price_column_i);
            break;
        case 6:
            if (custom_feature_added)
            { // remove custom feature
                main_ds = original_ds;
                column_names = column_names_org;

                if (test_ds)
                    dataset_free(test_ds);
                if (train_ds)
                    dataset_free(train_ds);
                if (weights_model)
                    weights_free(weights_model);

                test_ds = train_ds = weights_model = NULL;

                custom_feature_added = 0;
            }
            else
            { // add custom feature
                main_ds = cf_added_ds;
                column_names = column_names_cf;

                if (test_ds)
                    dataset_free(test_ds);
                if (train_ds)
                    dataset_free(train_ds);
                if (weights_model)
                    weights_free(weights_model);

                test_ds = train_ds = weights_model = NULL;

                custom_feature_added = 1;
            }
            break;
        case 7:
            continue_run = 0;
            break;

        default:
            printf("Enter a valid command\n");
            flush_stdin();
            wait_for_enter_key("continue");
            break;
        }
    }

    for (int i = 0; i < original_ds->max_cols; i++)
    {
        free(column_names_org[i]);
    }
    free(column_names_org);
    for (int i = 0; i < cf_added_ds->max_cols; i++)
    {
        free(column_names_cf[i]);
    }
    free(column_names_cf);

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
}
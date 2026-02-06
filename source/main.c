#include <stdio.h>
#include <plot_utils.h>
#include <input_utils.h>
#include <preprocess.h>

int draw_price_columns(Dataset *d, char **column_names, int column_count)
{
    static const int price_column_i = 8;

    for (int column = 0; column < column_count; column++)
    {

        double x[d->max_rows], y[d->max_rows];

        for (int i = 0; i < d->max_rows; i++)
        {
            x[i] = d->data[i][column];
            y[i] = d->data[i][price_column_i];
        }

        double slope, interception, correlation;
        analyze_points(x, y, d->max_rows, &slope, &interception, &correlation);
        char title[512];
        sprintf(title, "%s -> price", column_names[column]);
        printf("Ploting %s with correlation %lf : (y/n)\n", title, correlation);
        char confirm = getchar();
        getchar(); // for \n

        if (confirm == 'y')
        {

            // printf("x:%lf, y:%lf\n", x[0], y[0]);

            plot_points_with_regression(x, y, &slope, &interception, &correlation, d->max_rows, title, column_names[column], "price");
        }
    }
}

// @mhrohani1385
int main(void)
{
    char **column_names;
    int column_count;

    Dataset *d = read_from_csv_to_dataset(
        "housingprices.csv",
        (int[]){9},
        1,
        (OHE_Column[]){
            (OHE_Column){
                .ohe_values = (char *[]){"<1H OCEAN", "INLAND", "NEAR BAY", "NEAR OCEAN", "ISLAND"},
                .count = 5}},
        &column_names,
        &column_count);

    return 0;
}
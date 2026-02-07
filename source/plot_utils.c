#include <plot_utils.h>
#include <stdio.h>
#include <math.h>
#include <preprocess.h>
#include <input_utils.h>

#include <stdio.h>

void plot_actual_vs_predicted(double *x_real, double *y_pred, int count, double mse, double mape)
{
#ifndef _WIN32
    FILE *gp = popen("gnuplot", "w");
    if (!gp)
    {
        fprintf(stderr, "Error: could not open gnuplot\n");
        return;
    }

    fprintf(gp, "set terminal qt noenhanced\n");
    fprintf(gp, "set title 'Actual vs Predicted'\n");
    fprintf(gp, "set xlabel 'Actual Values'\n");
    fprintf(gp, "set ylabel 'Predicted Values'\n");
    fprintf(gp, "set grid\n");

    fprintf(gp, "set label 1 sprintf('MSE = %.6f', %f) at graph 0.02, 0.05\n", mse, mse);
    fprintf(gp, "set label 2 sprintf('MAPE = %.6f', %f) at graph 0.02, 0.10\n", mape, mape);

    fprintf(gp, "plot x title 'y = x' with lines lw 2 lc rgb 'red', '-' using 1:2 with points pt 7 ps 1.2 lc rgb 'blue' notitle\n");

    for (int i = 0; i < count; i++)
    {
        fprintf(gp, "%lf %lf\n", x_real[i], y_pred[i]);
    }

    fprintf(gp, "e\n");

    fflush(gp);

    flush_stdin();
    wait_for_enter_key("close plot");

    pclose(gp);
#endif
}

void plot_price_versus_features(Dataset *d, char **column_names, const int price_column_i)
{

    printf("You have entered Plot Mode : \n\n");

    double columns[d->max_cols][d->max_rows];
    double analysis[d->max_cols][3]; // slope , intercept , corellation

    // changing data arrange - writing data
    for (int i = 0; i < d->max_rows; i++)
    {
        double *row_data = d->data[i];
        for (int j = 0; j < d->max_cols; j++)
        {
            columns[j][i] = row_data[j];
        }
    }
    printf("-1 -> Exit Plot Mode\n");
    for (int i = 0; i < d->max_cols; i++)
    {
        if (i == price_column_i)
        {
            continue;
        }

        analyze_points(columns[i], columns[price_column_i], d->max_rows, &analysis[i][0], &analysis[i][1], &analysis[i][2]);
        char title[512];
        sprintf(title, "%2d -> Plot %s", i, column_names[i]);
        printf("%-37s (vs) price  :: correlation  %+lf\n", title, analysis[i][2]);
    }

    int p_i = 0;
    printf("\nPLOT:: ");
    scanf("%d", &p_i);

    while (p_i >= 0)
    {
        if (p_i > d->max_cols)
        {
            printf("Enter a valid plot id\n");
            flush_stdin();
            wait_for_enter_key("continue");
        }
        else
        {
            char title[512];
            sprintf(title, "%s (vs) price", column_names[p_i]);
            plot_points_with_regression(columns[p_i], columns[price_column_i], &analysis[p_i][0], &analysis[p_i][1], &analysis[p_i][2], d->max_rows, title, column_names[p_i], "price");
        }

        printf("PLOT:: ");
        scanf("%d", &p_i);
    }

    flush_stdin();

    printf("Exiting Plot Mode...\n");
}

void plot_points_with_regression(const double *x, const double *y, const double *slope, const double *intercept, const double *correlation, int n, const char *title, const char *x_lable, const char *y_label)
{
#ifndef _WIN32
    FILE *gp = popen("gnuplot", "w");
    if (!gp)
    {
        fprintf(stderr, "Error: could not open gnuplot.\n");
        return;
    }

    if (correlation)
    {
        fprintf(gp, "set title '%s -> correlation : %lf'\n", title, *correlation);
    }
    else
    {
        fprintf(gp, "set title '%s\n", title);
    }

    fprintf(gp, "set terminal qt noenhanced\n");
    fprintf(gp, "set xlabel '%s'\n", x_lable);
    fprintf(gp, "set ylabel '%s'\n", y_label);

    if (slope)
    {
        fprintf(gp, "plot '-' with points pointtype 5 pointsize 0.3 lc rgb 'blue' notitle"
                    ", %lf * x + %lf with lines lw 2 lc rgb 'red' notitle\n",
                *slope, *intercept);
    }
    else
    {
        fprintf(gp, "plot '-' with points pointtype 5 pointsize 0.3 lc rgb 'blue' notitle\n");
    }

    for (int i = 0; i < n; i++)
    {
        fprintf(gp, "%lf %lf\n", x[i], y[i]);
    }

    fprintf(gp, "e\n");

    fflush(gp);

    flush_stdin();
    wait_for_enter_key("close plot");

    pclose(gp);
#endif
}
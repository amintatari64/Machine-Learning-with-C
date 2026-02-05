#include <plot_utils.h>
#include <stdio.h>
#include <math.h>

// AI written
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

void plot_points_with_regression(const double *x, const double *y, int n, const char *title, const char *x_lable, const char *y_label)
{
    FILE *gp = popen("gnuplot -persistent", "w");
    if (!gp)
    {
        fprintf(stderr, "Error: could not open gnuplot.\n");
        return;
    }

    double line_intercept, line_slope, correlation;
    analyze_points(x, y, n, &line_slope, &line_intercept, &correlation);

    fprintf(gp, "set title '%s -> correlation : %lf'\n", title, correlation);
    fprintf(gp, "set xlabel '%s'\n", x_lable);
    fprintf(gp, "set ylabel '%s'\n", y_label);

    fprintf(gp, "plot '-' with points pointtype 7 pointsize 1 lc rgb 'blue' notitle, "
                "%lf * x + %lf with lines lw 2 lc rgb 'red' notitle\n",
            line_slope, line_intercept);

    for (int i = 0; i < n; i++)
    {
        fprintf(gp, "%lf %lf\n", x[i], y[i]);
    }

    fprintf(gp, "e\n");
    fflush(gp);
    pclose(gp);
}
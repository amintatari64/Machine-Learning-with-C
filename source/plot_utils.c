#include <plot_utils.h>
#include <stdio.h>
#include <math.h>
#include <preprocess.h>

void plot_points_with_regression(const double *x, const double *y, const double *slope, const double *intercept, const double *correlation, int n, const char *title, const char *x_lable, const char *y_label)
{
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

    fprintf(gp, "set title '%s'\n", title, correlation);
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

    getchar();

    pclose(gp);
}
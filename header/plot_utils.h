#ifndef PLOT_H
#define PLOT_H

void line_regression(const double *x, const double *y, int n, double* slope, double* intercept) ;
void plot_points(const double *x, const double *y, int n, const char *title, const char *x_lable, const char* y_label);

#endif
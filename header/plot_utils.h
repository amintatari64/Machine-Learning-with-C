#include<struct.h>

#ifndef PLOT_H
#define PLOT_H

void plot_points_with_regression(const double *x, const double *y, const double *slope, const double *intercept, const double *correlation, int n, const char *title, const char *x_lable, const char *y_label);

void plot_price_versus_features(Dataset *d, char **column_names, const int price_column_i);
void plot_actual_vs_predicted(double *x_real, double *y_pred, int count, double mse, double mape);

#endif
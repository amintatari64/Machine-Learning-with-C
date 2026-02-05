#include <stdio.h>

#include <plot_utils.h>

int main(void)
{
    double x[] = {1, 2, 3, 4, 5};
    double y[] = {1, 4, 9, 16, 25};

    plot_points(x, y, 5, "Sample Plot", "x", "y");

    return 0;
}
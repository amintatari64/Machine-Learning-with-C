#include <stdio.h>

#include <plot_utils.h>
#include <input_utils.h>

// @mhrohani1385
int main(void)
{
    read_from_csv_to_dataset("housingprices.csv",
                             (int[]){9},
                             1, (OHE_Column[]){(OHE_Column){.ohe_values = (char *[]){"<1H OCEAN", "INLAND", "NEAR BAY", "NEAR OCEAN"}, .count = 5}});
    return 0;
}
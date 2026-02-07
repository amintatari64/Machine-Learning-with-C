#include <struct.h>
#include <stdio.h>

#ifndef INPUT_U_H
#define INPUT_U_H

typedef struct
{
    char **ohe_values; // {"A" , "B" , "C"}
    int count;         // 3
} OHE_Column;

// read_from_csv_to_dataset("housingprices.csv",
//             (int[]){9, 10},
//              2,
//              (OHE_Column[]){(OHE_Column){
//.ohe_values = (char *[]){"<1H OCEAN", "INLAND", "NEAR BAY", "NEAR OCEAN"}, .count = 5}
// });

// including one hot encouding
Dataset *read_from_csv_to_dataset(
    const char *csv_file,
    int *ohe_indexes,
    int ohe_count,
    const OHE_Column *ohe_maps,
    char ***column_names);
Dataset *read_from_stream_to_dataset(
    FILE *stream,
    int *ohe_indexes,
    int ohe_count,
    const OHE_Column *ohe_maps,
    char ***columnNames);

void wait_for_enter_key(const char *message);

int cross_platform_kbhit(void);

char get_key_pressed();
void flush_stdin();

#endif
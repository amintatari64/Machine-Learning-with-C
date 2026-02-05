#include "struct.h"
#ifndef FILE_U_H
#define FILE_U_H

// including one hot encouding
void read_from_csv_to_dataset(const char* csv_file, Dataset* dataset_ptr, int* ohe_indexes, int ohe_count);

#endif
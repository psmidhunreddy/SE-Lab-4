#ifndef REPORT_H
#define REPORT_H

#include "student.h"

struct StudentResult {
    char student_id[20];
    char student_name[50];
    float percentage;
    float cgpa;
};

int generate_report_data(
    const char *input_file,
    int N,
    struct StudentResult results[],
    struct ErrorLog errors[],
    int *valid_count,
    int *error_count,
    float *class_avg,
    float *highest,
    float *lowest
);


#endif

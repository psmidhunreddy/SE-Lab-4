#include <string.h>
#include "../include/student.h"
#include "../include/grading.h"
#include "../include/report.h"

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
) {

    struct Student students[MAX_STUDENTS];

    int student_count = load_students(
        input_file,
        N,
        students,
        errors,
        error_count
    );



    if (student_count <= 0) return 0;

    float total_percentage = 0;
    *highest = 0;
    *lowest = 1000;

    for (int i = 0; i < student_count; i++) {

        int total = 0;

        for (int j = 0; j < SUBJECTS; j++) {
            total += students[i].minor_marks[j];
            total += students[i].major_marks[j];
        }

        students[i].total_marks = total;

        float percentage = calculate_percentage(total);
        float cgpa = calculate_cgpa(percentage);

        strcpy(results[i].student_id, students[i].student_id);
        strcpy(results[i].student_name, students[i].student_name);

        results[i].percentage = percentage;
        results[i].cgpa = cgpa;

        total_percentage += percentage;

        if (percentage > *highest) *highest = percentage;
        if (percentage < *lowest) *lowest = percentage;
    }

    *valid_count = student_count;
    *class_avg = total_percentage / student_count;

    return 1;
}

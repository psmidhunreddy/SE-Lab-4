#ifndef STUDENT_H
#define STUDENT_H

#define SUBJECTS 5
#define MAX_STUDENTS 100
#define MAX_ERRORS 100

struct Student {
    char student_id[20];
    char student_name[50];
    int minor_marks[SUBJECTS];
    int major_marks[SUBJECTS];
    int total_marks;
};

struct ErrorLog {
    char message[200];
};

int load_students(
    const char *filename,
    int N,
    struct Student students[],
    struct ErrorLog errors[],
    int *error_count
);

#endif

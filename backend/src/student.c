#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../include/student.h"
#include "../include/file_utils.h"

int is_valid_id(const char *id) {
    for (int i = 0; id[i] != '\0'; i++) {
        if (!isalnum(id[i]))
            return 0;
    }
    return 1;
}

int is_valid_name(const char *name) {
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]))
            return 0;
    }
    return 1;
}

int is_duplicate_id(const char *id, struct Student students[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].student_id, id) == 0)
            return 1;
    }
    return 0;
}

int load_students(
    const char *filename,
    int N,
    struct Student students[],
    struct ErrorLog errors[],
    int *error_count
) {

    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int count = 0;
    *error_count = 0;

    for (int attempt = 0; attempt < N; attempt++) {

        struct Student temp;

        if (fscanf(fp, "%19s %49s",
                   temp.student_id,
                   temp.student_name) != 2)
            break;  // file ended before N

        int valid = 1;

        // ID validation
        if (!is_valid_id(temp.student_id)) {

            sprintf(errors[*error_count].message,
                    "Invalid ID: %s",
                    temp.student_id);

            (*error_count)++;
            skip_line(fp);
            continue;
        }

        if (is_duplicate_id(temp.student_id, students, count)) {

            sprintf(errors[*error_count].message,
                    "Duplicate ID: %s",
                    temp.student_id);

            (*error_count)++;
            skip_line(fp);
            continue;
        }

        // Name validation
        if (!is_valid_name(temp.student_name)) {

            sprintf(errors[*error_count].message,
                    "Invalid name for ID %s",
                    temp.student_id);

            (*error_count)++;
            skip_line(fp);
            continue;
        }

        // Minor-Major pairs
        for (int i = 0; i < SUBJECTS; i++) {

            if (fscanf(fp, "%d", &temp.minor_marks[i]) != 1 ||
                temp.minor_marks[i] < 0 ||
                temp.minor_marks[i] > 40) {

                sprintf(errors[*error_count].message,
                        "Invalid minor marks for ID %s",
                        temp.student_id);

                (*error_count)++;
                skip_line(fp);
                valid = 0;
                break;
            }

            if (fscanf(fp, "%d", &temp.major_marks[i]) != 1 ||
                temp.major_marks[i] < 0 ||
                temp.major_marks[i] > 60) {

                sprintf(errors[*error_count].message,
                        "Invalid major marks for ID %s",
                        temp.student_id);

                (*error_count)++;
                skip_line(fp);
                valid = 0;
                break;
            }
        }

        if (!valid)
            continue;

        students[count++] = temp;
    }

    fclose(fp);
    return count;   // number of valid students
}

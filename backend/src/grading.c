#include "../include/grading.h"

float calculate_percentage(int total) {
    return total / 5.0;
}

float calculate_cgpa(float percentage) {
    return percentage / 10.0;
}

#include <math.h>
#include <stdio.h>
#include <malloc.h>


#define REPORT_MAX_LENGTH 32
#define error(...) (fprintf(stderr, __VA_ARGS__))
#define array_length(array) sizeof(array) / sizeof(array[0])

double get_average(double a, double b) { return (a + b) / 2; }

double f(double x) {
    return sin(x);
}

double calculate_area_using_rectangle_rule(double left_border, double right_border) {
    return f(get_average(left_border, right_border)) * (right_border - left_border);
}

double calculate_area_using_simpsons_rule(double left_border, double right_border) {
    return (right_border - left_border) / 6 *
           (f(left_border) + 4 * f(get_average(left_border, right_border)) + f(right_border));
}

double sum_partition_areas_calculated_by_rectangle_rule(
        double left_border,
        double right_border,
        unsigned int partition_count
) {
    double partition_length = (right_border - left_border) / partition_count;
    double current_left_border = left_border;
    double current_right_border = left_border + partition_length;
    double sum = 0;
    for (int i = 0; i < partition_count; i++) {
        sum += calculate_area_using_rectangle_rule(current_left_border, current_right_border);
        current_left_border += partition_length;
        current_right_border += partition_length;
    }
    return sum;
}

double sum_partition_areas_calculated_by_simpsons_rule(
        double left_border,
        double right_border,
        unsigned int partition_count
) {
    double partition_length = (right_border - left_border) / partition_count;
    double current_left_border = left_border;
    double current_right_border = left_border + partition_length;
    double sum = 0;
    for (int i = 0; i < partition_count; i++) {
        sum += calculate_area_using_simpsons_rule(current_left_border, current_right_border);
        current_left_border += partition_length;
        current_right_border += partition_length;
    }
    return sum;
}

char *get_integral_approximation_report(double left_border, double right_border, unsigned int partition_count) {
    char *report = malloc(REPORT_MAX_LENGTH * sizeof(char));
    if (report == NULL) {
        error("Cannot allocate memory for integral approximation report\n");
        return NULL;
    }
    if (snprintf(report, REPORT_MAX_LENGTH, "%d %.5f %.5f", partition_count,
                 sum_partition_areas_calculated_by_rectangle_rule(left_border, right_border, partition_count),
                 sum_partition_areas_calculated_by_simpsons_rule(left_border, right_border, partition_count)) == 0) {
        free(report);
        error("Cannot write integral approximation report to string\n");
        return NULL;
    }
    return report;
}

void free_string_array(char **array, size_t array_length) {
    for (size_t j = 0; j < array_length; j++) {
        free(array[j]);
    }
    free(array);
}

char **get_integral_approximation_report_array(
        double left_border,
        double right_border,
        unsigned int *partition_counts,
        size_t experiments_count
) {
    char **report_array = malloc(sizeof(char *) * experiments_count);
    if (report_array == NULL) {
        error("Cannot allocate memory for integral approximation report array\n");
        return NULL;
    }
    for (size_t i = 0; i < experiments_count; i++) {
        report_array[i] = get_integral_approximation_report(left_border, right_border, partition_counts[i]);
        if (report_array[i] == NULL) {
            free_string_array(report_array, i);
            return NULL;
        }
    }
    return report_array;
}

int read_interval(double *left_border, double *right_border) {
    if (printf("Enter interval's left border: \n") < 0) {
        error("Cannot write to stdout\n");
        return -1;
    }
    if (scanf("%lf", left_border) != 1) {
        error("Cannot read interval's left border\n");
        return -1;
    }
    if (*left_border < 0) {
        error("Left border of the interval must be greater than or equal to 0\n");
        return -1;
    }
    if (printf("Enter interval's right border: \n") < 0) {
        error("Cannot write to stdout\n");
        return -1;
    }
    if (scanf("%lf", right_border) != 1) {
        error("Cannot read interval's right border\n");
        return -1;
    }
    if (*right_border > M_PI) {
        error("Right border of the interval must be less than or equal to pi\n");
        return -1;
    }
    if (*left_border > *right_border) {
        error("Right border of the interval must be greater than left\n");
        return -1;
    }
    return 0;
}

int main() {
    double left_border;
    double right_border;
    if (read_interval(&left_border, &right_border) != 0) {
        return 1;
    }
    unsigned int partition_counts[] = {6, 10, 20, 100, 500, 1000};
    size_t experiment_count = array_length(partition_counts);
    char **report_array = get_integral_approximation_report_array(left_border, right_border, partition_counts,
                                                                  experiment_count);
    if (report_array == NULL) return 1;
    for (size_t i = 0; i < experiment_count; i++) {
        if (printf("%s\n", report_array[i]) < 0) {
            free_string_array(report_array, experiment_count);
            return 1;
        }
    }
    free_string_array(report_array, experiment_count);
    return 0;
}

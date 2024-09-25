#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cjson/cJSON.h>

#define MAX_ROOTS 100

// Function to convert a string in any base to decimal
long long int convert_to_decimal(const char* value, int base) {
    return strtoll(value, NULL, base);
}

// Function to perform Gaussian elimination
void gaussian_elimination(int n, double matrix[MAX_ROOTS][MAX_ROOTS + 1]) {
    for (int i = 0; i < n; i++) {
        // Find pivot
        int max_element = i;
        for (int j = i + 1; j < n; j++) {
            if (fabs(matrix[j][i]) > fabs(matrix[max_element][i])) {
                max_element = j;
            }
        }

        // Swap maximum row with current row
        for (int k = i; k <= n; k++) {
            double temp = matrix[max_element][k];
            matrix[max_element][k] = matrix[i][k];
            matrix[i][k] = temp;
        }

        // Make all rows below this one 0 in current column
        for (int j = i + 1; j < n; j++) {
            double factor = matrix[j][i] / matrix[i][i];
            for (int k = i; k <= n; k++) {
                matrix[j][k] -= matrix[i][k] * factor;
            }
        }
    }
}

// Function to find the solution using back substitution
double back_substitution(int n, double matrix[MAX_ROOTS][MAX_ROOTS + 1]) {
    double x[MAX_ROOTS];
    for (int i = n - 1; i >= 0; i--) {
        x[i] = matrix[i][n];
        for (int j = i + 1; j < n; j++) {
            x[i] -= matrix[i][j] * x[j];
        }
        x[i] /= matrix[i][i];
    }
    return x[0];  // The constant term
}

int main() {
    FILE* file = fopen("input.json", "r");
    if (file == NULL) {
        printf("Failed to open input file\n");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* json_string = malloc(file_size + 1);
    fread(json_string, 1, file_size, file);
    fclose(file);

    json_string[file_size] = '\0';

    cJSON* json = cJSON_Parse(json_string);
    if (json == NULL) {
        const char* error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL) {
            printf("JSON parsing error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        free(json_string);
        return 1;
    }

    cJSON* keys = cJSON_GetObjectItemCaseSensitive(json, "keys");
    int n = cJSON_GetObjectItemCaseSensitive(keys, "n")->valueint;
    int k = cJSON_GetObjectItemCaseSensitive(keys, "k")->valueint;

    double matrix[MAX_ROOTS][MAX_ROOTS + 1] = {0};

    for (int i = 0; i < n; i++) {
        char key[10];
        sprintf(key, "%d", i + 1);
        cJSON* root = cJSON_GetObjectItemCaseSensitive(json, key);
        if (root != NULL) {
            int x = atoi(key);
            int base = atoi(cJSON_GetObjectItemCaseSensitive(root, "base")->valuestring);
            char* value = cJSON_GetObjectItemCaseSensitive(root, "value")->valuestring;
            long long int y = convert_to_decimal(value, base);

            for (int j = 0; j < k; j++) {
                matrix[i][j] = pow(x, k - j - 1);
            }
            matrix[i][k] = y;
        }
    }

    gaussian_elimination(k, matrix);
    double c = back_substitution(k, matrix);

    printf("%.0f\n", c);

    cJSON_Delete(json);
    free(json_string);
    return 0;
}
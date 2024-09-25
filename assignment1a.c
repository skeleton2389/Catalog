#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_ROOTS 100
#define MAX_VALUE_LENGTH 100

typedef struct {
    int x;
    long double y;
} Point;

long long decode_value(char* value, int base) {
    long long result = 0;
    int len = strlen(value);
    for (int i = 0; i < len; i++) {
        int digit;
        if (isdigit(value[i])) {
            digit = value[i] - '0';
        } else {
            digit = toupper(value[i]) - 'A' + 10;
        }
        result = result * base + digit;
    }
    return result;
}

long double lagrange_interpolation(Point* points, int k, long double x) {
    long double result = 0.0;
    for (int i = 0; i < k; i++) {
        long double term = points[i].y;
        for (int j = 0; j < k; j++) {
            if (j != i) {
                term = term * (x - points[j].x) / (points[i].x - points[j].x);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    char line[1000];
    int n, k;
    Point points[MAX_ROOTS];
    int point_count = 0;

    // Read input
    while (fgets(line, sizeof(line), stdin)) {
        if (strstr(line, "\"n\":")) {
            sscanf(line, "\"n\": %d", &n);
        } else if (strstr(line, "\"k\":")) {
            sscanf(line, "\"k\": %d", &k);
        } else if (strstr(line, "\"base\":")) {
            int base;
            char value[MAX_VALUE_LENGTH];
            sscanf(line, "\"base\": \"%d\",", &base);
            fgets(line, sizeof(line), stdin);
            sscanf(line, "\"value\": \"%[^\"]\"", value);
            
            points[point_count].x = point_count + 1;
            points[point_count].y = (long double)decode_value(value, base);
            point_count++;
        }
    }

    // Calculate constant term using Lagrange interpolation
    long double c = lagrange_interpolation(points, k, 0);

    // Print the result
    printf("%.0Lf\n", c);

    return 0;
}
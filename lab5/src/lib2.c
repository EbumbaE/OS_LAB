#include "../include/lib.h"

float Pi(int k) {
    float result = 1;
    for (int i = 1; i <= k; i++){
        result *= (float)(4 * i * i) / (float)((2 * i + 1) * (2 * i - 1)); 
    }
    return 2 * result;
}

float Square(float a, float b) {
    return a * b / 2.0;
}
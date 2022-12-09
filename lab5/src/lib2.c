#include "../include/lib.h"

os_float Pi(os_int k) {
    os_float result = 1;
    for (os_int i = 1; i <= k; i++){
        result *= (os_float)(4 * i * i) / (os_float)((2 * i + 1) * (2 * i - 1)); 
    }
    return 2 * result;
}

os_float Square(os_float a, os_float b) {
    return a * b / 2.0;
}
#include "../include/lib.h"

int minusAlternation(int i){
    if (i % 2 == 0)
        return 1;
    return -1;  
}

float Pi(int k) {
    float result = 0;
    for (int i = 0; i < k; i++){
        result += (float)(minusAlternation(i)) / (float)(2 * i + 1); 
    }
    return 4 * result;
}

float Square(float a, float b) {
    return a * b;
}
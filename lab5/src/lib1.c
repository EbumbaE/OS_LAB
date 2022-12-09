#include "../include/lib.h"

os_int minusAlternation(os_int i){
    if (i % 2 == 0)
        return 1;
    return -1;  
}

os_float Pi(os_int k) {
    os_float result = 0;
    for (os_int i = 0; i < k; i++){
        result += (os_float)(minusAlternation(i)) / (os_float)(2 * i + 1); 
    }
    return 4 * result;
}

os_float Square(os_float a, os_float b) {
    return a * b;
}
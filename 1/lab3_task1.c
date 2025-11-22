#include "lab3_task1.h"
#include <stdlib.h>


int bitwise_add(int a, int b) {
    while (b != 0) {
        int carry = a & b; // вычисляем перенос
        a = a ^ b;         // складываем биты без переноса
        b = carry << 1;    // сдвигаем перенос влево
    }
    return a;
}

// увеличение на 1
int bitwise_increment(int a) {
    return bitwise_add(a, 1);
}

status_code convert_to_base_2r(unsigned int number, int r, char** result) {
    if (r < 1 || r > 5 || result == NULL) {
        return INVALID_INPUT;
    }

    *result = (char*)malloc(sizeof(char) * 64);
    if (*result == NULL) {
        return MEMORY_ERROR;
    }

    char* buffer = *result;
    
    const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUV";

    if (number == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return OK;
    }
 
    unsigned int mask = ~((~0U) << r);
    
    int index = 0;

    while (number != 0) {
        unsigned int digit = number & mask;
        
        buffer[index] = alphabet[digit];
        
        index = bitwise_increment(index);
        
        number >>= r;
    }

    buffer[index] = '\0';

    int start = 0;
    int end = bitwise_add(index, -1);

    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        
        start = bitwise_increment(start);
        end = bitwise_add(end, -1);
    }

    return OK;
}
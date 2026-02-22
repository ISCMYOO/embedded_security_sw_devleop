#include "common.h"

void logHex(const uint8_t *hex, size_t size){
    for (size_t i = 0; i < size; i++) {
        printf("%02X ", hex[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    if (size % 16 != 0) printf("\n");
}
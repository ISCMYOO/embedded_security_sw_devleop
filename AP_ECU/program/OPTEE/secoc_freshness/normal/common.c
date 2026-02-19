#include "common.h"

uint32_t str_to_u32(const char* buf){
    char* endptr;
    errno = 0;

    unsigned long val = strtoul(buf, &endptr, 10);

    if(endptr == buf){
        printf("No digits were found\n");
        return 0;
    }

    if(errno == ERANGE || val > UINT32_MAX){
        printf("Value out of range\n");
        return 0;
    }

    return (uint32_t)val;
}
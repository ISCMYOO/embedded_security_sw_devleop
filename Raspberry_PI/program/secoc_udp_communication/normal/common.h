#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <sys/random.h>

#define MAX_PAYLOAD 256
#define KEY_SIZE    16

uint8_t get_number();

int get_random(uint8_t* out, uint32_t len);

int parse_uint32(const char *input, uint32_t *out_value);
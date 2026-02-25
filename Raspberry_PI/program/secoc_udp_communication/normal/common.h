#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <sys/random.h>

#define MAX_PAYLOAD 256
#define KEY_SIZE    16

#define CMD_RESET_FRESHNESS     1
#define CMD_SEND_MESSAGE        2
#define CMD_REPLAY_PACKET       3
#define CMD_READ_FRESHNESS      4
#define CMD_SET_FRESHNESS       5
#define CMD_LOAD_FRESHNESS      6
#define CMD_DELETE_OBJECT       7
#define CMD_READ_KEY            8
#define CMD_LOAD_KEY            9
#define CMD_SYNC_KEY            10
#define CMD_GEN_SYNC_KEY        11
#define CMD_GEN_MAC             12
#define CMD_EXIT                13

uint8_t get_number();

int get_random(uint8_t* out, uint32_t len);

int parse_uint32(const char *input, uint32_t *out_value);
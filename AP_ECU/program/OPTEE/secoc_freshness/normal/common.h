#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>

#define CMD_RESET_FRESHNESS     1
#define CMD_SEND_MESSAGE        2
#define CMD_REPLAY_PACKET       3
#define CMD_READ_FRESHNESS      4
#define CMD_SET_FRESHNESS       5
#define CMD_LOAD_FRESHNESS      6
#define CMD_DELETE_FRESHNESS    7

uint32_t str_to_u32(const char* buf);
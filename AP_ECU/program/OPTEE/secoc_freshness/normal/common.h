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
#define CMD_READ_KEY            8
#define CMD_LOAD_KEY            9
#define CMD_SYNC_KEY            10
#define CMD_GEN_SYNC_KEY        11
#define CMD_GEN_MAC             12
#define CMD_EXIT                13

uint32_t str_to_u32(const char* buf);
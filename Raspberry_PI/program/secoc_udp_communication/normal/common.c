#include "common.h"

uint8_t get_number(){
	char buf[64];
	char *end;
	long choice;

	if(!fgets(buf, sizeof(buf), stdin))
		return -1;

	choice = strtol(buf, &end, 10);

	if(end == buf || (*end != '\n' && *end != '\0'))
		return -1;

	return (uint8_t)choice;
}

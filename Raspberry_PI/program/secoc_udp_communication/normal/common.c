#include "common.h"

int parse_uint32(const char *input, uint32_t *out_value){
	char *endptr;
	int len;

	if(input == NULL || out_value == NULL)	return -1; // not input

	len = strlen(input);

	if(len > 0 && input[len - 1] == '\n')	len--;
	if(len == 0)	return -1; // emtpy input

	for(int i = 0; i < len; i++){
		if(!isdigit((unsigned char)input[i])){
			return -1;
		}
	}

	errno = 0;
	unsigned long value = strtoul(input, &endptr, 10);

	if(errno == ERANGE)	return -1;
	if(value > UINT32_MAX)	return -1;

	*out_value = (uint32_t)value;
	return 0;
}

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

int get_random(uint8_t* out, uint32_t len){
	ssize_t ret = getrandom(out, len, 0);
	if(ret != len){
		perror("getrandom");
		return -1;
	}
	return 0;
}
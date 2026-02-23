CC = gcc
TARGET = secoc_udp_sender

SRCS = secoc_udp_sender.c \
	  udpcom.c \
	  common.c \
	  secoc.c

CFLAGS = -Wall -Wextra -Wno-deprecated-declarations
LDFLAGS = -lcrypto

all:
	$(CC) $(SRCS) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
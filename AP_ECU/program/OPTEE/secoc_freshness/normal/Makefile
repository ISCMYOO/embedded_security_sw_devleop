CC = arm-ostl-linux-gnueabi-gcc

SYSROOT = /home/iscmyoo/project/mobility_security/optee_stm32/Developer_Package/SDK/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi

TARGET = secoc_udp_receiver_freshness_optee

SRCS = secoc_udp_receiver_freshness_optee.c \
       secoc.c \
       udpcom.c \
       ta_api.c \
       common.c \

CFLAGS = -mthumb \
         -mfpu=neon-vfpv4 \
         -mfloat-abi=hard \
         -mcpu=cortex-a7 \
         -D_TIME_BITS=64 \
         -D_FILE_OFFSET_BITS=64 \
         --sysroot=$(SYSROOT) \
         -I../include


LDFLAGS = -lteec

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
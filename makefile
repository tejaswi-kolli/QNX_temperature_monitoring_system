CC = qcc
TARGET = gcc_ntoaarch64le

CFLAGS = -V$(TARGET) -Wall
INCLUDES = -Iinclude

SRCDIR = src

all: sensor_process logger_process cli_process

sensor_process:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCDIR)/sensor_process.c -o sensor_process

logger_process:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCDIR)/logger_process.c -o logger_process

cli_process:
	$(CC) $(CFLAGS) $(INCLUDES) $(SRCDIR)/cli_process.c -o cli_process

clean:
	rm -f sensor_process logger_process cli_process


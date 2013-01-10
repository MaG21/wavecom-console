#ifndef DEVICE_H_
#define DEVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <stdint.h>

#define   LF              0x0A
#define   CR              0x0D
#define   NUL             0x00
#define   CTRL_Z          0x1A

#define   BAUD            B115200
#define   MODEM_MODE      0x01
#define   BUFFER_LENGTH   0xFF

struct device {
	struct termios  old_properties; 
	int             fd;
	uint8_t         status;
};

/* prototipes */
int              open_device(char *, struct device *);
int              device_write_string(int, char *);
void             close_device(struct device *);
void             get_cmd_stdin(char *, size_t);
size_t           print_results(struct device *);
#endif

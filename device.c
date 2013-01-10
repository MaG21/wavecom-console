#include "device.h"

/*
 * returns:
 *    -1 in case of error and errno is set appropriately.
 *    otherwise returns a file descriptor.
 */
int
open_device(char *dev_path, struct device *dev)
{
	int             fd;
	struct termios  term;

	fd = open(dev_path, O_RDWR | O_NOCTTY);

	if(fd < 0)
		return -1;

	if(tcgetattr(fd, &term) < 0)
		return -1;

	dev->fd = fd;
	(void)memcpy(&dev->old_properties, &term, sizeof(term));

	/*set BAUD rate */
	cfsetispeed(&term, BAUD);
	cfsetospeed(&term, BAUD);

	/* set term flags */
	term.c_cflag     &= ~(CSIZE | PARENB);
	term.c_cflag     |= (CS8 | CREAD | CLOCAL);
	term.c_lflag     &= ~(ECHO | ECHOE | ICANON | ISIG);
	term.c_oflag     &= ~OPOST;
	term.c_iflag     &= ~(INPCK | INLCR | ICRNL);

	/* One second */
	term.c_cc[VMIN]  = 0;
	term.c_cc[VTIME] = 10;

	if(tcsetattr(fd, TCSANOW, &term) < 0) {
		close(fd);
		return -1;
	}

	/* clear input */
	tcflush(fd, TCIFLUSH);
   return fd;
}

/*
 * returns:
 *    on success, the number of bytes written.
 *    on error, -1 is returned, and errno is set appropriately.
 */
device_write_string(int fd, char *data)
{
	return write(fd, data, strlen(data));
}

/*
 * returns:
 *    on success, the character read as a short int.
 *    on error, -1 and errno is set appropriately.
 */
static int16_t
read_byte(struct device *dev)
{
	int      ret;
	uint16_t c;

	ret = read(dev->fd, &c, 1);

	switch(ret) {
	case 0:
	case -1:
		return ret;
	case 1:
		return c;
	default:
		fprintf(stderr, "read_byte(): error, overflow.\n");
		close_device(dev);
		exit(EXIT_FAILURE);
	}
}

/*
 *
 */
size_t
print_results(struct device *dev)
{
	int             ret;
	fd_set          master;
	size_t          nbytes  =0;
	int16_t         c;
	int16_t         old     =0;
	struct timeval  tv;

	FD_ZERO(&master);
	FD_SET(dev->fd, &master);

	printf("\n");

	/* unset modem mode */
	dev->status = 0;

	while( 1 ) {

		tv.tv_sec   = 0;
		tv.tv_usec  = 805999; /* magic? :P */

		ret = select(dev->fd+1, &master, NULL, NULL, &tv);

		if(ret<=0) { 

			if(ret<0) {
				perror("select()");
				exit(EXIT_FAILURE);
			}

			if(c==' ' && old=='>') {
				printf("\b\b");
				dev->status = MODEM_MODE;
			}

			return nbytes;
		}

		old = c;

		c = read_byte(dev);

		if(c<0) { 
			perror("read_byte()");
			exit(EXIT_FAILURE);
		}

		if(dev->status!=MODEM_MODE)
			putchar(c);
		nbytes++;
	}
}


void
get_cmd_stdin(char *buf, size_t lenth)
{
	int nbytes;

	nbytes = read(STDIN_FILENO, buf, lenth);

	if(nbytes<0) {
		perror("read()");
		exit(EXIT_FAILURE);
	}

	buf[nbytes--]	= NUL;
	buf[nbytes]	= CR;

  return;
}

void
proc_cmd(char *cmd, uint8_t *status)
{
	for(;*cmd!='\0'; cmd++) {

		if(*cmd=='.') {
			*cmd=CTRL_Z;
			*status	= 0;
		}
	}
  return;
}


/* close device properly */
void
close_device(struct device *dev)
{
	tcsetattr(dev->fd, TCSANOW, &dev->old_properties);
	close(dev->fd);
  return;
}


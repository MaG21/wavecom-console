#include "device.h"

int
main(int argc, char **argv)
{
	int           ret;
	char          cmd[BUFFER_LENGTH+1];
	struct device dev;

	if(argc!=2) {
		fprintf(stderr, "Use: %s <device path>\n\n\t"
		                "ie: %s /dev/ttyUSB0\n", argv[0], argv[0]);
		return EXIT_FAILURE;
	}

	printf("Version 0.1.0\nCopyright(C) 2013, Manuel A. Guilamo.\n"
	       "THIS SOFTWARE COMES WITH ABSOLUTELY NO WARRANTY!"
	       " USE AT YOUR OWN RISK!\n\n");

	ret = open_device(argv[1], &dev);

	if(ret<0) {
		perror("Error open_device()");
		return EXIT_FAILURE;
	}

	dev.status = 0;

	while (1) {
		if(dev.status==MODEM_MODE) {
			printf("Wavecom > ");
		} else {
			printf("(AT Command) >> ");
		}

		fflush(stdout);

		get_cmd_stdin(&cmd[0], BUFFER_LENGTH);

		if(dev.status!=MODEM_MODE) {
			if( strcmp(&cmd[0], "quit\r")==0) {
				puts("exiting...");
				close_device(&dev);
				return EXIT_SUCCESS;
			}
		}

		proc_cmd(&cmd[0], &dev.status);

		if(dev.status!=MODEM_MODE)
			puts(&cmd[0]);

		ret = device_write_string(dev.fd, &cmd[0]);

		if(ret<0) {
			perror("device_write_string()");
			close_device(&dev);
			exit(EXIT_FAILURE);
		}

		printf(">>> %d bytes written.\n", ret);

		if(dev.status!=MODEM_MODE) {
			printf(">>> waiting for device...");
			fflush(stdout);
		}

		ret = print_results(&dev);

		if(dev.status!=MODEM_MODE)
			puts("|Ok|");

		printf(">>> %d bytes read.\n\n", ret);
	}
}







CC=gcc
CFLAGS=-O2 -v

wavecom-console:
	$(CC) -c -o device.o device.c $(CFLAGS)
	$(CC) -c -o main.o main.c $(CFLAGS)
	$(CC) main.o device.o -o wavecom-console $(CFLAGS)

.PHONY: README.md COPYRIGHT device.c device.h main.c

clean:
	rm -f *.o wavecom-console

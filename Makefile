CC=gcc

CFLAGS = -c -std=c99

all: peer

peer: p2p.o diagnose.o adaptive.o communication.o 
	$(CC) -o peer p2p.o diagnose.o adaptive.o communication.o -lpthread

clean:
	rm *.o
	rm peer

%.o: %.c
	$(CC) $(CFLAGS) $*.c

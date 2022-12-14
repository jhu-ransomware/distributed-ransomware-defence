CC=gcc

CFLAGS = -c -std=c99

all: peer

peer: p2p.o diagnose.o adaptive.o communication.o 
	$(CC) -o peer p2p.o diagnose.o adaptive.o communication.o -lpthread

test: test.o monitor.o entrophy.o
	$(CC) -o test_entr test.o monitor.o entrophy.o

clean:
	rm *.o
	rm peer

%.o: %.c
	$(CC) $(CFLAGS) $*.c

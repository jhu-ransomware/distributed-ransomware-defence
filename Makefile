CC=gcc

CFLAGS = -c -std=c99

all: peer

peer: p2p.o diagnose.o adaptive.o communication.o hash.o entrophy.o monitor.o
	$(CC) -o peer p2p.o diagnose.o adaptive.o communication.o hash.o entrophy.o -lpthread monitor.o -lm

test: test.o monitor.o entrophy.o
	$(CC) -o test_entr test.o monitor.o entrophy.o -lm

clean:
	rm *.o
	rm peer
	rm test_entr

%.o: %.c
	$(CC) $(CFLAGS) $*.c

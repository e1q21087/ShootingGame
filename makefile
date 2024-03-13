CC = cc
CFLAGS = -Wall
LDFLAGS = -lncurses

all: shooting

shooting: main.o game.o sentence.o
	$(CC) -o shooting main.o game.o sentence.o $(LDFLAGS)
	rm -f main.o game.o sentence.o

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

game.o: game.c
	$(CC) $(CFLAGS) -c -o game.o game.c

sentence.o: sentence.c
	$(CC) $(CFLAGS) -c -o sentence.o sentence.c

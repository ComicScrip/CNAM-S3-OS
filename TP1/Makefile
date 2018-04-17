CC=gcc
CFLAGS=-W -Wall -ansi -pedantic


all: tp1-exo1 tp1-exo2 tp1-exo3

tp1-exo1: tp1-exo1.o 
	$(CC) -o bin/tp1-exo1 src/tp1-exo1.o 

tp1-exo1.o: tp1-exo1.c
	$(CC) -o src/tp1-exo1.o -c src/tp1-exo1.c -$(CFLAGS)


tp1-exo2: tp1-exo2.o 
	$(CC) -o bin/tp1-exo2 src/tp1-exo2.o 

tp1-exo2.o: tp1-exo2.c
	$(CC) -o src/tp1-exo2.o -c src/tp1-exo2.c $(CFLAGS)


tp1-exo3: tp1-exo3.o 
	$(CC) -o bin/tp1-exo3 src/tp1-exo3.o 

tp1-exo3.o: tp1-exo3.c
	$(CC) -o src/tp1-exo31.o -c src/tp1-exo3.c $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

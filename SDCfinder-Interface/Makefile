
TEST_OBJ = xor.o #matrix.o

LIBDIR = ../lib
INCDIR = ../include

CC = gcc
AR = ar

all: main

#matrix.o: matrix.c Makefile
#	$(CC) -g -I$(INCDIR) -c matrix.c 
#
xor.o: xor.c Makefile
	$(CC) -g -I$(INCDIR) -c xor.c

main: main.c $(TEST_OBJ)
	$(CC) -g -I$(INCDIR) main.c xor.o -L$(LIBDIR) -lsdcmf -lcrypto -o main

clean:
	rm -f $(TEST_OBJ) main *~

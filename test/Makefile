
PROGRAM = matrix

LIBDIR = ../lib
INCDIR = ../include

CC = gcc
AR = ar

all: $(PROGRAM)

matrix: matrix.c Makefile
	$(CC) -g -I$(INCDIR) matrix.c -L$(LIBDIR) -lsdcmf -lcrypto -o matrix

clean:
	rm -f $(PROGRAM) *~

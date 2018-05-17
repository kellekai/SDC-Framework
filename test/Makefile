
PROGRAMS = xor matrix

LIBDIR = ../lib
INCDIR = ../include

CC = gcc
AR = ar

all: $(PROGRAMS)

matrix: matrix.c Makefile
	$(CC) -g -I$(INCDIR) matrix.c -L$(LIBDIR) -lsdcmf -lcrypto -o matrix

xor: xor.c Makefile
	$(CC) -g -I$(INCDIR) xor.c -L$(LIBDIR) -lsdcmf -lcrypto -o xor

clean:
	rm -f $(PROGRAMS) *~

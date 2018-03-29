
LIB = sdcmf

TARGET = lib$(LIB).a
INCDIR = ../include
OBJS = sdcmf.o sdcmf_hash.o sdcmf_message.o
DEPS = sdcmf.c sdcmf_hash.c sdcmf_message.c $(INCDIR)/sdcmf.h sdcmf_internal.h

CC = gcc
AR = ar

all: $(TARGET)

%.o : %.c $(DEPS) Makefile
	$(CC) -g -I$(INCDIR) -I. -O3 -c $< -o $@

$(TARGET): $(OBJS)
	$(AR) rsc $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS) *~

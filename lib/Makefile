
LIB = sdcmf

TARGET = lib$(LIB).a
INCDIR = ../include
OBJS = sdcmf.o sdcmf_hash.o sdcmf_message.o

CC = gcc
AR = ar

all: $(TARGET)

%.o : %.c
	$(CC) -I$(INCDIR) -I. -O3 -c $< -o $@

$(TARGET): $(OBJS)
	$(AR) rsc $(TARGET) $(OBJS)

clean:
	rm -f $(TARGET) $(OBJS) *~

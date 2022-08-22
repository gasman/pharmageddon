CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter

all: demo.o

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

.PHONY: all
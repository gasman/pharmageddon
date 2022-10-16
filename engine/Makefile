CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
OBJS = gfx.o gfx3d.o

all: engine.a

engine.a: $(OBJS)
	ar rcs $@ $^

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

.PHONY: all

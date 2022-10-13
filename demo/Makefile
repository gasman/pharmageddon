CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
OBJS = demo.o gfx.o plasma.o boxes.o rotozoom.o crosstunnel.o credits.o ambulance.o planes.o boards.o video.o gfx3d.o

all: demo.a

demo.a: $(OBJS)
	ar rcs $@ $^

%.o : %.c
	$(CC) -I$(RGB_INCDIR) $(CFLAGS) -c -o $@ $<

.PHONY: all

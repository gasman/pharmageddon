CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
OBJS = demo.o plasma.o patarty.o rotozoom.o credits.o ambulance.o planes.o boards.o video1.o scroller.o title.o jarig.o physics.o badapple.o twister.o stniccc.o prescription.o nyancat.o amigaball.o farjan.o
ENGINE_DIR = ../engine

all: demo.a

demo.a: $(OBJS)
	ar rcs $@ $^

%.o : %.c
	$(CC) -I$(ENGINE_DIR) $(CFLAGS) -c -o $@ $<

.PHONY: all

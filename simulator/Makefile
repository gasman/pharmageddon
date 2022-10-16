# A simple Makefile for compiling small SDL projects

# set the compiler
CC := clang

DEMO_DIR = ../demo
ENGINE_DIR = ../engine

CFLAGS := `sdl2-config --cflags` -ggdb3 -O0 --std=c99 -Wall -I $(DEMO_DIR) -I $(ENGINE_DIR)
LDFLAGS := `sdl2-config --libs` -lSDL2_mixer -lm -lassimp

# add header files here
HDRS := $(DEMO_DIR)/demo.h

# add source files here
SRCS := main.c

# generate names of object files
OBJS := $(SRCS:.c=.o) $(DEMO_DIR)/demo.a $(ENGINE_DIR)/engine.a

# name of executable
EXEC := pharma

# default recipe
all: $(EXEC)

$(DEMO_DIR)/demo.a: FORCE
	$(MAKE) -C $(DEMO_DIR)

$(ENGINE_DIR)/engine.a: FORCE
	$(MAKE) -C $(ENGINE_DIR)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean FORCE

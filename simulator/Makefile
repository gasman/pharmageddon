# A simple Makefile for compiling small SDL projects

# set the compiler
CC := clang

CFLAGS := `sdl2-config --cflags` -ggdb3 -O0 --std=c99 -Wall
LDFLAGS := `sdl2-config --libs`
# add header files here
HDRS := effect.h

# add source files here
SRCS := main.c effect.c

# generate names of object files
OBJS := $(SRCS:.c=.o)

# name of executable
EXEC := pharma

# default recipe
all: $(EXEC)
 
# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

# recipe for building object files
#$(OBJS): $(@:.o=.c) $(HDRS) Makefile
#    $(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

# recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean

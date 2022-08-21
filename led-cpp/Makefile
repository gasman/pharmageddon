CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter
CXXFLAGS=$(CFLAGS)
OBJECTS=pharma.o effect.o
BINARIES=pharma

# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=../../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

pharma: pharma.o

effect.o: effect.c
	gcc -c effect.c

# All the binaries that have the same name as the object file.q
% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)


%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE


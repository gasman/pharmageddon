
# Where our library resides. You mostly only need to change the
# RGB_LIB_DISTRIBUTION, this is where the library is checked out.
RGB_LIB_DISTRIBUTION=../../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread `sdl2-config --libs` -lSDL2_mixer

DEMO_DIR = ../demo

CFLAGS=-Wall -O3 -g -Wextra -Wno-unused-parameter `sdl2-config --cflags`
CXXFLAGS=$(CFLAGS)
OBJECTS=pharma.o $(DEMO_DIR)/demo.a
BINARIES=pharma

all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

# All the binaries that have the same name as the object file.q
% : %.o $(RGB_LIBRARY)
	$(CXX) $< -o $@ $(LDFLAGS)

# Since the C example uses the C++ library underneath, which depends on C++
# runtime stuff, you still have to also link -lstdc++
pharma : $(OBJECTS) $(RGB_LIBRARY)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) -lstdc++

$(DEMO_DIR)/demo.a: FORCE
	$(MAKE) -C $(DEMO_DIR)

%.o : %.c
	$(CC) -I$(RGB_INCDIR) -I$(DEMO_DIR) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE

CC=g++
CPPFLAGS=-g -I.

# add stubs for non-beaglebone env
ifeq ($(shell uname -m),x86_64)
  CPPFLAGS+= -I./stubs -DPIANO_SIMULATOR
endif

# right now a change to any header file will cause everything to get rebuilt
DEPS = $(shell ls *.h)

# assume all *.cpp files should be included
OBJ = $(shell ls *.cpp simulator/*.cpp | sed "s/cpp$$/o/")

all: $(OBJ)
	$(CC) -o piano $^ $(CPPFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $(CPPFLAGS) $<

.PHONY: clean

clean:
	rm -f $(OBJ) piano



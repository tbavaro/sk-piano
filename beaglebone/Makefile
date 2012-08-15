CC=g++
LDFLAGS=
CPPFLAGS=$(shell find . -type d | sed "s/^/-I/")

# add stubs for non-beaglebone env
ifeq ($(shell uname -m),x86_64)
  CPPFLAGS+= -I./stubs -DPIANO_SIMULATOR
endif

# right now a change to any header file will cause everything to get rebuilt
DEPS = $(shell find . -name "*.h")

# assume all *.cpp files should be included
OBJ = $(shell find . -name "*.cpp" | sed "s/cpp$$/o/")

all: $(OBJ)
	$(CC) -o piano $^ $(LDFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $(CPPFLAGS) $<

.PHONY: clean

clean:
	rm -f $(shell find . -name "*.o") piano



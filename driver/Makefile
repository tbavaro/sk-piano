CC=g++
CPPFLAGS=-g -I.
EXEC=driver

# add stubs for non-beaglebone env
ifneq ($(shell uname -m),armv7l)
  CPPFLAGS+= -I./stubs -DNOT_BEAGLEBONE
endif

# right now a change to any header file will cause everything to get rebuilt
DEPS = $(shell ls *.h)

# assume all *.cpp files should be included
OBJ = $(shell ls *.cpp | sed "s/cpp$$/o/")

all: $(OBJ)
	$(CC) -o $(EXEC) $^ $(CPPFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $(CPPFLAGS) $<

.PHONY: clean

clean:
	rm -f $(OBJ) $(EXEC)



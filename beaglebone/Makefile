CC=g++
CPPFLAGS=-I.
DEPS = 
OBJ = \
      main.o \
      BeagleBone.o \
      Colors.o \
      LightStrip.o \
      Util.o

all: $(OBJ)
	$(CC) -o piano $^ $(CPPFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o piano



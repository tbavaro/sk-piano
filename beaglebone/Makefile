CC=g++
CPPFLAGS=-I.
DEPS = 
OBJ = \
      BeagleBone.o \
      ColorCycleParticle.o \
      Colors.o \
      CometVisualizer.o \
      LightStrip.o \
      MasterVisualizer.o \
      MovingParticle.o \
      ParticleVisualizer.o \
      Piano.o \
      PianoDelegate.o \
      SimpleMovingParticle.o \
      SimpleParticleVisualizer.o \
      SimpleVisualizer.o \
      Util.o \
      Visualizer.o \
      main.o


all: $(OBJ)
	$(CC) -o piano $^ $(CPPFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o piano



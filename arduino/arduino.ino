#include "Piano.h"
#include "SimpleVisualizer.h"
#include "SimpleParticleVisualizer.h"
#include "CometVisualizer.h"
#include "LPD8806.h"
#include "SPI.h"
#include "Colors.h"
#include "MasterVisualizer.h"
#include "Utils.h"
#include <avr/pgmspace.h>


// Number of RGB LEDs in strand:
static int nLEDs = 160;

static LPD8806 strip(nLEDs);
static MasterVisualizer master_viz(&strip);
static Piano piano(&master_viz);

static Visualizer* makeSimpleParticleVisualizer() {
  return new SimpleParticleVisualizer(&strip, 300);
}

static Visualizer* makeCometVisualizer() {
  return new CometVisualizer(&strip, 300);
}

static Visualizer* makeSimpleVisualizer() {
  return new SimpleVisualizer(&strip);
}

void setup() {
  Serial.begin(9600);

  // initialize LED strip
  strip.begin();
  strip.show();

  // add visualizers
  master_viz.addVisualizer(makeSimpleParticleVisualizer);
  master_viz.addVisualizer(makeCometVisualizer);
  master_viz.addVisualizer(makeSimpleVisualizer);
}

static uint8_t counter = 0;

void loop() {
  ++counter;
  piano.checkOne();

  if (counter == 0) {
    Serial.print("free memory: ");
    Serial.println(Utils::availableMemory());
  }
}

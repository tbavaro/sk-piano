#include "Piano.h"
#include "SimpleVisualizer.h"
#include "SimpleParticleVisualizer.h"
#include "LPD8806.h"
#include "SPI.h"
#include "Colors.h"
#include "MasterVisualizer.h"

static Piano* piano;

// Number of RGB LEDs in strand:
static int nLEDs = 160;

static LPD8806 strip(nLEDs);
static MasterVisualizer master_viz(&strip);

void setup() {
  Serial.begin(9600);

  Colors::init();
  
  // initialize LED strip
  strip.begin();
  strip.show();
  
  MasterVisualizer* master_viz = new MasterVisualizer(&strip);
  master_viz->addVisualizer(new SimpleParticleVisualizer(&strip, 300));
  master_viz->addVisualizer(new SimpleVisualizer(&strip));
  
  piano = new Piano(master_viz);
}

void loop() {
  piano->checkOne();
}

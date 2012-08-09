#include "Piano.h"
#include "SimpleVisualizer.h"
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
  
  PianoDelegate* simple_viz = new SimpleVisualizer(&strip);
  
  piano = new Piano(simple_viz);
}

void loop() {
  piano->checkOne();
}

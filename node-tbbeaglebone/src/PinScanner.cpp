#include "PinScanner.h"

#include <unistd.h>

using namespace std;

#define FOR_EACH_PIN(i, v, block) \
  for(vector<Pin*>::const_iterator _##i = v.begin(); _##i != v.end(); ++_##i) {\
    Pin& i = (** _##i); \
    block \
  }

PinScanner::PinScanner(
    const vector<Pin*>& outputPins, const vector<Pin*>& inputPins)
        : outputPins(outputPins), inputPins(inputPins) {
  this->reset();
}

PinScanner::~PinScanner() {}

void PinScanner::reset() {
  FOR_EACH_PIN(pin, inputPins, {
    pin.setMode(Pin::INPUT);
  });

  FOR_EACH_PIN(pin, outputPins, {
    pin.setMode(Pin::OUTPUT);
  });

  // disable pins; doing it after we set all the modes lets us be more efficient
  // if we have to shell out to make the pins accessible to non-root users
  FOR_EACH_PIN(pin, outputPins, {
    pin.setMode(Pin::OUTPUT);
  });
}

vector<int> PinScanner::scan() {
  vector<int> results;

  int index = 0;
  FOR_EACH_PIN(outputPin, outputPins, {
    outputPin.write(true);

    // wait 1ms to give it time to take effect
    // TODO see if we can make this shorter
    usleep(500);

    FOR_EACH_PIN(inputPin, inputPins, {
      if (inputPin.read()) {
        results.push_back(index);
      }

      ++index;
    });
  });

  return results;
}

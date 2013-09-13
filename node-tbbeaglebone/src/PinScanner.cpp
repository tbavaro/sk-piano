#include "PinScanner.h"

#include <stdint.h>
#include <sys/time.h>
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
    pin.write(false);
  });
}

static inline uint32_t now() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return ((tv.tv_sec % 86400) * 1000 + tv.tv_usec / 1000);
}

vector<int> PinScanner::scan() {
  uint32_t startTime = now();

  vector<int> results;

  int index = 0;
  FOR_EACH_PIN(outputPin, outputPins, {
    outputPin.write(true);

    // wait 1ms to give it time to take effect
    // TODO see if we can make this shorter
//    usleep(500);
    uint32_t readStartTime = now();

    FOR_EACH_PIN(inputPin, inputPins, {
      if (inputPin.read()) {
        results.push_back(index);
      }

      ++index;
    });

    uint32_t readEndTime = now();

    printf("read took %d ms\n", readEndTime - readStartTime);

    outputPin.write(false);
  });

  uint32_t endTime = now();

  printf("took %d ms\n", endTime - startTime);

  return results;
}

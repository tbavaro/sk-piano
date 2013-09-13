#ifndef __INCLUDED_PIN_SCANNER_H
#define __INCLUDED_PIN_SCANNER_H

#include "Pin.h"

#include <vector>

class PinScanner {
  public:
    PinScanner(
        const std::vector<Pin*>& outputPins,
        const std::vector<Pin*>& inputPins);
    ~PinScanner();

    void reset();
    std::vector<int> scan();

  private:
    const std::vector<Pin*> outputPins;
    const std::vector<Pin*> inputPins;
};

#endif

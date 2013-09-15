#include "Pin.h"
#include "RuntimeException.h"

#include <set>
#include <sstream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>

using namespace std;

const string& Pin::modeName(Mode mode) {
  static string none("NONE");
  static string input("INPUT");
  static string output("OUTPUT");

  switch(mode) {
    case NONE:    return none;
    case INPUT:   return input;
    case OUTPUT:  return output;
    default:
      throw RuntimeException("invalid mode: %d", mode);
  }
}

Pin::Pin(int number) : number(number), mode(NONE) {
}

Pin::~Pin() {
  this->close();
}

void Pin::close() {
}

void Pin::setMode(Mode _mode) {
  this->mode = _mode;
}

std::string Pin::toString() const {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "pin%d(%s)", number, modeName(mode).c_str());
  return buffer;
}

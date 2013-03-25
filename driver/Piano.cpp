#include "Piano.h"
#include "Util.h"

#include <stdio.h>
#include <string.h>

static const char keyNames[] = 
    { 'a', 'A', 'b', 'c', 'C', 'd', 'D', 'e', 'f', 'F', 'g', 'G' };
static const int num_keyNames = sizeof(keyNames) / sizeof(char);

Piano::Piano() : keyValues(new uint8_t[Piano::NUM_KEYS]) {
  // initialize all keys to be OFF
  for (int i = 0; i < Piano::NUM_KEYS; ++i) {
    keyValues[i] = false;
  }
}

Piano::~Piano() {
  delete[] keyValues;
}

void Piano::printKeys() {
  static int counter = 0;
  ++counter;

  char buf[256] = { 0 };
  char* ptr = buf;
  sprintf(ptr, "%d:", counter);
  ptr += strlen(ptr);
  for (int i = 0; i < Piano::NUM_KEYS; ++i) {
    if (keyValues[i]) {
      *ptr++ = keyNames[i % num_keyNames];
    } else {
      *ptr++ = ' ';
    }
  }
  Util::log("%s", buf);
}


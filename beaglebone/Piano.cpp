#include "Piano.h"
#include "Util.h"

#include <stdio.h>
#include <string.h>

static const char key_names[] = 
    { 'a', 'A', 'b', 'c', 'C', 'd', 'D', 'e', 'f', 'F', 'g', 'G' };
static const int num_key_names = sizeof(key_names) / sizeof(char);

Piano::Piano(PianoDelegate* delegate) 
    : delegate(delegate), key_values(new uint8_t[Piano::NUM_KEYS]) {
  // initialize all keys to be OFF
  for (int i = 0; i < Piano::NUM_KEYS; ++i) {
    key_values[i] = false;
  }
}

Piano::~Piano() {
  delete[] key_values;
}

void Piano::printKeys() {
  static int counter = 0;
  ++counter;

  char buf[256] = { 0 };
  char* ptr = buf;
  sprintf(ptr, "%d:", counter);
  ptr += strlen(ptr);
  for (int i = 0; i < Piano::NUM_KEYS; ++i) {
    if (key_values[i]) {
      *ptr++ = key_names[i % num_key_names];
    } else {
      *ptr++ = ' ';
    }
  }
  Util::log("%s", buf);
}


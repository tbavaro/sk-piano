#include "Pin.h"
#include "RuntimeException.h"

#include <set>
#include <sstream>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>

using namespace std;

//#define NOISY

#ifdef IS_SIMULATOR
#define NOISY
static set<string> existingFilenames;
#endif

static inline string toString(int value) {
  char buffer[64];
  snprintf(buffer, sizeof(buffer), "%d", value);
  return buffer;
}

static inline bool fileExists(const string& filename) {
#ifdef IS_SIMULATOR
  return (existingFilenames.find(filename) != existingFilenames.end());
#endif

  struct stat buffer;
  return (stat(filename.c_str(), &buffer) == 0);
}

static void writeFile(const char* filename, const string& contents) {
#ifdef NOISY
  printf("writing [%s]: %s\n", filename, contents.c_str());
#endif
#ifdef IS_SIMULATOR
  return;
#endif

  FILE* f = fopen(filename, "w");
  if (f == NULL) {
    throw RuntimeException("unable to open file for writing: %s", filename);
  }

  bool success = (fwrite(contents.c_str(), contents.size(), 1, f) > 0);

  fclose(f);

  if (!success) {
    throw RuntimeException("unable to write to file: %s", filename);
  }
}

static inline void writeFile(const string& filename, const string& contents) {
  writeFile(filename.c_str(), contents);
}

static inline string getGpioPath(int pinNumber) {
  return "/sys/class/gpio/gpio" + toString(pinNumber);
}

template <typename T>
static inline bool contains(const vector<T>& v, T value) {
  for (typename vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
    if (*i == value) {
      return true;
    }
  }
  return false;
}

typedef struct {
  int number;
  Pin::Mode mode;
} AwaitingPinEntry;

static inline AwaitingPinEntry* findMatchingPinEntry(
    vector<AwaitingPinEntry>& v, int pinNumber) {
  for (vector<AwaitingPinEntry>::iterator i = v.begin(); i != v.end(); ++i) {
    if (i->number == pinNumber) {
      return &(*i);
    }
  }
  return NULL;
}

static vector<int> pinNumbersWithNonRootAccess;
static vector<AwaitingPinEntry> pinsAwaitingNonRootAccess;

static void setPinDirection(int number, Pin::Mode mode) {
  const char* direction = (mode == Pin::INPUT ? "in" : "out");
  writeFile(getGpioPath(number) + "/direction", direction);
}

static void setPinDirectionMaybeDeferred(int number, Pin::Mode mode) {
  // if the pin already has non-root access, set direction now
  if (contains(pinNumbersWithNonRootAccess, number)) {
    setPinDirection(number, mode);
    return;
  }

  // if it's already deferred then just make sure we'll eventually set the
  // right mode
  AwaitingPinEntry* entry =
      findMatchingPinEntry(pinsAwaitingNonRootAccess, number);
  if (entry != NULL) {
    entry->mode = mode;
    return;
  }

  // otherwise, add it to be set later
  AwaitingPinEntry newEntry = { number, mode };
  pinsAwaitingNonRootAccess.push_back(newEntry);
}

static void processPinsAwaitingNonRootAccess() {
  // nothing to do if nobody's waiting
  if (pinsAwaitingNonRootAccess.empty()) {
    return;
  }

  stringstream ss;
  ss << "sudo /usr/local/bin/allow_nonroot_gpio_pin_access.sh";
  for (vector<AwaitingPinEntry>::iterator i = pinsAwaitingNonRootAccess.begin();
       i != pinsAwaitingNonRootAccess.end(); ++i) {
    ss << " " << toString(i->number);
  }
#ifdef NOISY
  printf("exec: %s\n", ss.str().c_str());
#endif
#ifndef IS_SIMULATOR
  int rc = system(ss.str().c_str());
  bool success = (rc == 0);
  if (!success) {
    throw RuntimeException("unable to grant non-root access to pins: %d", rc);
  }
#endif

  for (vector<AwaitingPinEntry>::iterator i = pinsAwaitingNonRootAccess.begin();
       i != pinsAwaitingNonRootAccess.end(); ++i) {
    pinNumbersWithNonRootAccess.push_back(i->number);
    setPinDirection(i->number, i->mode);
  }

  pinsAwaitingNonRootAccess.clear();
}

Pin::Pin(const string& name, int number) : name(name), number(number) {
  mode = NONE;
  gpioPath = getGpioPath(number);
  valueFilename = gpioPath + "/value";
  valueFile = NULL;
}

Pin::~Pin() {
  close();
}

void Pin::close() {
  if (valueFile != NULL) {
    fclose(valueFile);
    valueFile = NULL;
  }
}

void Pin::setMode(Mode mode) {
  // do nothing if we're already set to this mode
  if (mode == this->mode) {
    return;
  }

  // close the value file if it's open
  close();

  this->mode = mode;

  // export the pin if necessary
  if (!fileExists(gpioPath)) {
    writeFile("/sys/class/gpio/export", toString(number));
#ifdef IS_SIMULATOR
    existingFilenames.insert(gpioPath);
#endif
    if (!fileExists(gpioPath)) {
      throw RuntimeException("unable to export pin %d", number);
    }
  }

  setPinDirectionMaybeDeferred(number, mode);
}

// TODO check this out for being faster
// http://stackoverflow.com/questions/13124271/driving-beaglebone-gpio-through-dev-mem

bool Pin::read() {
  if (mode != INPUT) {
    throw RuntimeException("pin is not set to INPUT mode: %d", number);
  }

  processPinsAwaitingNonRootAccess();

  FILE* f = fopen(valueFilename.c_str(), "rb");
  if (f == NULL) {
    throw RuntimeException("unable to open file for reading: %s",
        valueFilename.c_str());
  }

  char value = fgetc(f);
  fclose(f);

  bool result = (value == '1');

#ifdef NOISY
  printf("read pin %d: %d\n", number, result);
#endif

  return result;
}

void Pin::write(bool value) {
  if (mode != OUTPUT) {
    throw RuntimeException("pin is not set to OUTPUT mode: %d", number);
  }

  processPinsAwaitingNonRootAccess();

#ifdef NOISY
  printf("writing pin %d: %d\n", number, value);
#endif

  if (valueFile == NULL) {
    valueFile = fopen(valueFilename.c_str(), "w");
    if (valueFile == NULL) {
      throw RuntimeException("unable to open file for writing: %s",
          valueFilename.c_str());
    }
  }

  fputc(value ? '1' : '0', valueFile);
  fflush(valueFile);
}

const string& Pin::getName() const {
  return name;
}

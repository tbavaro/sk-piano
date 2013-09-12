#include "pin.h"
#include "runtime_exception.h"

#include <set>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>

using namespace std;

#define IS_SIMULATOR

#ifdef IS_SIMULATOR
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
#ifdef IS_SIMULATOR
  printf("writing [%s]: %s\n", filename, contents.c_str());
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
#ifdef IS_SIMULATOR
  printf("exec: %s\n", ss.str().c_str());
#else
  bool success = (system(ss.str().c_str()) == 0);
  if (!success) {
    throw RuntimeException("unable to grant non-root access to pins");
  }
#endif

  for (vector<AwaitingPinEntry>::iterator i = pinsAwaitingNonRootAccess.begin();
       i != pinsAwaitingNonRootAccess.end(); ++i) {
    pinNumbersWithNonRootAccess.push_back(i->number);
    setPinDirection(i->number, i->mode);
  }

  pinsAwaitingNonRootAccess.clear();

  printf("ok\n");
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

bool Pin::read() {
  if (mode != INPUT) {
    throw RuntimeException("pin is not set to INPUT mode: %d", number);
  }

  processPinsAwaitingNonRootAccess();

  FILE* f = fopen(valueFilename.c_str(), "r");
  if (f == NULL) {
    throw RuntimeException("unable to open file for reading: %s",
        valueFilename.c_str());
  }

  uint8_t value;
  bool success = (fread(&value, sizeof(value), 1, f) > 0);

  fclose(f);

  if (!success) {
    throw RuntimeException("unable to read file: %s", valueFilename.c_str());
  }

  return (value == '1');
}

void Pin::write(bool value) {
  if (mode != OUTPUT) {
    throw RuntimeException("pin is not set to OUTPUT mode: %d", number);
  }

  processPinsAwaitingNonRootAccess();

  if (valueFile == NULL) {
    valueFile = fopen(valueFilename.c_str(), "w");
    if (valueFile == NULL) {
      throw RuntimeException("unable to open file for writing: %@",
          valueFilename.c_str());
    }
  }

  uint8_t valueByte = (value ? '1' : '0');
  bool success = (fwrite(&valueByte, sizeof(valueByte), 1, valueFile) > 0);
  if (!success) {
    throw RuntimeException("unable to write file: %@", valueFilename.c_str());
  }

  fflush(valueFile);
}

void Pin::test() {
  setPinDirectionMaybeDeferred(44, INPUT);
  setPinDirectionMaybeDeferred(47, INPUT);
  setPinDirectionMaybeDeferred(44, INPUT);
  processPinsAwaitingNonRootAccess();
  setPinDirectionMaybeDeferred(44, INPUT);
  processPinsAwaitingNonRootAccess();
}

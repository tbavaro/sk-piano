#include "BeagleBone.h"
#include "PhysicalPiano.h"
#include "SKTypes.h"
#include "Util.h"

#include <string.h>

static Pin* note_pins[] = {
  &Pin::pin(8, 15),
  &Pin::pin(8, 13),
  &Pin::pin(8, 23),
  &Pin::pin(8, 21),
  &Pin::pin(8, 26),
  &Pin::pin(8, 24),
  &Pin::pin(8, 19),
  &Pin::pin(8, 17),
  &Pin::pin(8, 25),
  &Pin::pin(8, 18),
  &Pin::pin(8, 20),
  &Pin::pin(8, 22)
};
static const uint8_t num_note_pins = sizeof(note_pins) / sizeof(note_pins[0]);

static Pin* octavePins[] = { 
  &Pin::pin(8, 12),
  &Pin::pin(8, 14),
  &Pin::pin(8, 16),
  &Pin::pin(8, 7),
  &Pin::pin(8, 9),
  &Pin::pin(8, 11),
  &Pin::pin(8, 10),
  &Pin::pin(8, 8)
};
static const uint8_t num_octavePins = sizeof(octavePins) / sizeof(octavePins[0]);

/**
 * Mappings from pins to keys.
 *
 * note_mappings[<i>] = <note> where:
 *   i = octave_pin * num_note_pins + note_pin   ("unmapped key")
 *   note = note value from 0 to (Piano::NUM_KEYS - 1)
 **/
static const Key key_mappings[] = {
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,
    13,  12,  16,  14,  23,  22,  18,  15,  17,  19,  21,  20,
    25,  24,  34,  35,  30,  28,  31,  32,  33,  26,  27,  29,
    44,  47,  41,  45,  37,  38,  39,  40,  36,  43,  46,  42,
    59,  58,  50,  56,  53,  48,  52,  57,  51,  55,  49,  54,
    71,  70,  63,  60,  66,  65,  64,  61,  62,  67,  68,  69,
    82,  83,  73,  74,  78,  81,  75,  76,  72,  80,  79,  77,
     0,   0,   0,   0,  85,  86,   0,   0,   0,   0,  84,  87
};

static const int key_history_max_length = 88;
static int key_history[key_history_max_length];
static int key_history_length = 0;

PhysicalPiano::PhysicalPiano() 
    : Piano() {
  // initialize current octave and note so the first pin will be next  
  currentOctave = num_octavePins - 1;
  currentNote = num_note_pins - 1;

  // "unmapped key" is the index into key_mappings; it should be
  // equal to currentOctave * num_note_pins + currentNote.  This
  // is probably cheaper to keep as a separate value than to
  // do the multiplication on every pass.
  currentUnmappedKey = num_octavePins * num_note_pins - 1;
  
  // set up note pins as INPUT pins
  for (int i = 0; i < num_note_pins; ++i) {
    note_pins[i]->setPinMode(INPUT);
  }
  
  // set up octave pins as OUTPUT pins and set them to LOW
  for (int i = 0; i < num_octavePins; ++i) {
    octavePins[i]->setPinMode(OUTPUT);
    octavePins[i]->digitalWrite(OFF);
  }
}

void PhysicalPiano::scan() {
  currentUnmappedKey = -1;
  for(currentOctave = 0; currentOctave < num_octavePins; ++currentOctave) {
    octavePins[currentOctave]->digitalWrite(ON);
    Util::delay(1); // do we need this?
    for(currentNote = 0; currentNote < num_note_pins; ++currentNote) {
      currentUnmappedKey++;
  
      Key key = key_mappings[currentUnmappedKey];
      int value = note_pins[currentNote]->digitalRead();

      if (value && key_history_length < key_history_max_length) {
        bool found = false;
        for (int i = 0; i < key_history_length; ++i) {
          if (key_history[i] == currentUnmappedKey) {
            found = true;
            break;
          }
        }

        if (!found) {
          fprintf(stderr, "key %d at position %d\n", key_history_length, currentUnmappedKey);
          key_history[key_history_length++] = currentUnmappedKey;
          if (key_history_length == key_history_max_length) {
            int map[96] = { 0 };
            for (int i = 0; i < key_history_length; ++i) {
              map[key_history[i]] = i;
            }
            for (int i = 0; i < 96; ++i) {
              fprintf(stderr, "%d, ", map[i]);
            }
            fprintf(stderr, "\n");
          }
        } else {
          fprintf(stderr, "duplicate key %d\n", currentUnmappedKey);
        }
      }
      
      if (value != keyValues[key]) {
        keyValues[key] = value;
      }
    }
    octavePins[currentOctave]->digitalWrite(OFF);
  }
}

int PhysicalPiano::fillPressedKeys(uint8_t* keys) {
  int n = 0;
  for (int key = 0; key < Piano::NUM_KEYS; ++key) {
    if (keyValues[key]) {
      keys[n++] = key;
    }
  }
  return n;
}

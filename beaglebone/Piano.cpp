#include "BeagleBone.h"
#include "Piano.h"
#include "Util.h"

#include <string.h>

static const uint8_t num_keys = 88;

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

static Pin* octave_pins[] = { 
  &Pin::pin(8, 12),
  &Pin::pin(8, 14),
  &Pin::pin(8, 16),
  &Pin::pin(8, 7),
  &Pin::pin(8, 9),
  &Pin::pin(8, 11),
  &Pin::pin(8, 10),
  &Pin::pin(8, 8)
};
static const uint8_t num_octave_pins = sizeof(octave_pins) / sizeof(octave_pins[0]);

static const char key_names[] = 
    { 'a', 'A', 'b', 'c', 'C', 'd', 'D', 'e', 'f', 'F', 'g', 'G' };
static const int num_key_names = sizeof(key_names) / sizeof(char);

/**
 * Mappings from pins to keys.
 *
 * note_mappings[<i>] = <note> where:
 *   i = octave_pin * num_note_pins + note_pin   ("unmapped key")
 *   note = note value from 0 to (num_keys - 1)
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

Piano::Piano(PianoDelegate* delegate) 
    : delegate(delegate), key_values(new uint8_t[num_keys]) {
  // initialize current octave and note so the first pin will be next  
  current_octave = num_octave_pins - 1;
  current_note = num_note_pins - 1;

  // "unmapped key" is the index into key_mappings; it should be
  // equal to current_octave * num_note_pins + current_note.  This
  // is probably cheaper to keep as a separate value than to
  // do the multiplication on every pass.
  current_unmapped_key = num_octave_pins * num_note_pins - 1;
  
  // set to true if any values changed, reset at the end of a full pass
  changed_since_last_pass = false;

  // set up note pins as INPUT pins
  for (int i = 0; i < num_note_pins; ++i) {
    note_pins[i]->setPinMode(INPUT);
  }
  
  // set up octave pins as OUTPUT pins and set them to LOW
  for (int i = 0; i < num_octave_pins; ++i) {
    octave_pins[i]->setPinMode(OUTPUT);
    octave_pins[i]->digitalWrite(OFF);
  }
  
  // initialize all keys to be OFF
  for (int i = 0; i < num_keys; ++i) {
    key_values[i] = OFF;
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
  for (int i = 0; i < num_keys; ++i) {
    if (key_values[i]) {
      *ptr++ = key_names[i % num_key_names];
    } else {
      *ptr++ = ' ';
    }
  }
  Util::log("%s", buf);
}

bool Piano::scan() {
  changed_since_last_pass = false;  
  current_unmapped_key = 0;
  for(current_octave = 0; current_octave < num_octave_pins; ++current_octave) {
    octave_pins[current_octave]->digitalWrite(ON);
    Util::delay(1); // do we need this?
    for(current_note = 0; current_note < num_note_pins; ++current_note) {
      current_unmapped_key++;
  
      Key key = key_mappings[current_unmapped_key];
      int value = note_pins[current_note]->digitalRead();
      
      if (value != key_values[key]) {
        key_values[key] = value;
        changed_since_last_pass = true;
        
        if (value) {
          delegate->onKeyDown(key);
        } else {
          delegate->onKeyUp(key);
        }
      }
    }
    octave_pins[current_octave]->digitalWrite(OFF);
  }

  delegate->onPassFinished(changed_since_last_pass);

  return changed_since_last_pass;
}

void Piano::checkOne() {
  // Step forward one note, moving to the next octave if necessary.
  // If we finish a complete pass, print out the keys if anything 
  // changed this pass.
  current_note++;
  current_unmapped_key++;
  if (current_note == num_note_pins) {    
    // turn off the previous octave
    octave_pins[current_octave]->digitalWrite(OFF);
    
    // go to the first note of the next octave
    current_note = 0;
    current_octave++;
    if (current_octave == num_octave_pins) {
      delegate->onPassFinished(changed_since_last_pass);
      changed_since_last_pass = false;
      current_octave = 0;
      current_unmapped_key = 0;
    }
    
    // turn on the new octave
    octave_pins[current_octave]->digitalWrite(ON);
    
    // TODO maybe we don't need this delay, or maybe it should be longer
    Util::delay(1);
  }
  
  Key key = key_mappings[current_unmapped_key];
  int value = note_pins[current_note]->digitalRead();
  if (value != key_values[key]) {
    key_values[key] = value;
    changed_since_last_pass = true;
    
    if (value) {
      delegate->onKeyDown(key);
    } else {
      delegate->onKeyUp(key);
    }
  }  
}
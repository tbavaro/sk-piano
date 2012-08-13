#include "Arduino.h"
#include "Piano.h"
#include <avr/pgmspace.h>

static const uint8_t num_keys = 88;

static const uint8_t note_pins[] = 
    { 30, 28, 38, 36, 41, 39, 34, 32, 40, 33, 35, 37 };
static const uint8_t num_note_pins = sizeof(note_pins) / sizeof(uint8_t);

static const uint8_t octave_pins[] = { 27, 29, 31, 22, 24, 26, 25, 23 };
static const uint8_t num_octave_pins = sizeof(octave_pins) / sizeof(uint8_t);

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
static const Key PROGMEM key_mappings[] = {
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
    : delegate(delegate), key_values((uint8_t*)malloc(num_keys)) {
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
    pinMode(note_pins[i], INPUT);
  }
  
  // set up octave pins as OUTPUT pins and set them to LOW
  for (int i = 0; i < num_octave_pins; ++i) {
    pinMode(octave_pins[i], OUTPUT);
    digitalWrite(octave_pins[i], LOW);
  }
  
  // initialize all keys to be OFF
  for (int i = 0; i < num_keys; ++i) {
    key_values[i] = LOW;
  }  
}

Piano::~Piano() {
  free(key_values);
}

void Piano::printKeys() {
  static int counter = 0;
  ++counter;
  Serial.print(counter);
  Serial.print(':');
  for (int i = 0; i < num_keys; ++i) {
    if (key_values[i]) {
      Serial.print(key_names[i % num_key_names]);
    } else {
      Serial.print(' ');
    }
  }
  Serial.println();
}

void Piano::checkOne() {
  // Step forward one note, moving to the next octave if necessary.
  // If we finish a complete pass, print out the keys if anything 
  // changed this pass.
  current_note++;
  current_unmapped_key++;
  if (current_note == num_note_pins) {    
    // turn off the previous octave
    digitalWrite(octave_pins[current_octave], LOW);
    
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
    digitalWrite(octave_pins[current_octave], HIGH);
    
    // TODO maybe we don't need this delay, or maybe it should be longer
    delay(1);
  }
  
  Key key = pgm_read_byte_near(key_mappings + current_unmapped_key);
  int value = digitalRead(note_pins[current_note]);
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

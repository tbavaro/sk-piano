#include "BeagleBone.h"
#include "PhysicalPiano.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

typedef uint32_t Command;

static const Command CMD_OK   = *((const Command*)"OK  ");
static const Command CMD_SCAN = *((const Command*)"SCAN");
static const Command CMD_KEYS = *((const Command*)"KEYS");
static const Command CMD_SHOW = *((const Command*)"SHOW");

static void log(const char* msg) {
  fprintf(stderr, "%s\n", msg);
}

static void fatal(const char* msg) {
  log(msg);
  exit(1);
}

class Message {
  public:
    Message(uint32_t length, uint8_t* buffer) : length(length), buffer(buffer) {};
    ~Message() {
      if (buffer != NULL) {
        delete[] buffer;
        buffer = NULL;
      }
    };

    const uint32_t length;
    const uint8_t* buffer;
    
    static Message* read(FILE* f);
};

Message* Message::read(FILE* f) {
  uint32_t length;
  if (fread(&length, sizeof(length), 1, f) != 1) {
    fatal("EOF reading length");
  }

  uint8_t* buffer = new uint8_t[length];
  if (fread(buffer, length, 1, f) != 1) {
    fatal("EOF reading message");
  }

  return new Message(length, buffer);
}

static FILE* outPipe = NULL;
static SPI* spi = NULL;
static PhysicalPiano* piano = NULL;

static void sendMessage(Command cmd, const uint8_t* body, uint32_t bodyLength) {
  uint32_t totalLength = bodyLength + sizeof(cmd);
  if (fwrite(&totalLength, sizeof(totalLength), 1, outPipe) != 1 ||
      fwrite(&cmd, sizeof(cmd), 1, outPipe) != 1 ||
      (bodyLength > 0 && fwrite(body, bodyLength, 1, outPipe) != 1)) {
    fatal("error sending message");
  }
  fflush(outPipe);
}

static void sendMessage(Command cmd) {
  sendMessage(cmd, NULL, 0);
}

static void processScanMessage() {
  uint8_t keys[88];
  uint8_t numPressedKeys = 0;
  if (piano != NULL) {
    piano->scan();
    numPressedKeys = piano->fillPressedKeys(keys);
  }
  sendMessage(CMD_KEYS, keys, numPressedKeys);
}

static void processShowMessage(const uint8_t* body, uint32_t bodyLength) {
  int numPixels = bodyLength / 4;
  int numBytes = numPixels * 3 + 1;
  uint8_t bytes[numBytes];
  const uint8_t* in = body;
  uint8_t* out = bytes;
  for (int i = 0; i < numPixels; ++i) {
    in++;
    *(out++) = 0x80 | *(in++);
    *(out++) = 0x80 | *(in++);
    *(out++) = 0x80 | *(in++);
  }
  bytes[numBytes - 1] = 0;
  if (spi != NULL) {
    spi->send(bytes, numBytes);
  }
}

static void processMessage(Command cmd, const uint8_t* body, uint32_t bodyLength) {
  if (cmd == CMD_SCAN) {
    processScanMessage();
  } else if (cmd == CMD_SHOW) {
    processShowMessage(body, bodyLength);
  } else {
    printf("unknown command: %.4s (bodyLength=%d)\n", (char*)&cmd, bodyLength);
  }
}

static void processMessage(const Message& message) {
  Command cmd = *((Command*)message.buffer);
  const uint8_t* body = (message.buffer + sizeof(Command));
  uint32_t bodyLength = (message.length - sizeof(Command));
  processMessage(cmd, body, bodyLength);
}

/**
 * Reads stdin for commands related to setting the status of the LEDs,
 * and outputs key activity to stdout.
 *
 * Simple binary message format in both directions: 4 bytes for length 
 * of message followed by that many bytes of message body
 */
int main(int argc, char** argv) {
  if (argc < 3) {
    fatal("need to specify pipe names");
  }

  FILE* inPipe = fopen(argv[1], "rb+");
  outPipe = fopen(argv[2], "wb+");

#ifndef NOT_BEAGLEBONE
  spi = new SPI(4e6);
  piano = new PhysicalPiano();
#endif

  sendMessage(CMD_OK);
  while(1) {
    Message* msg = Message::read(inPipe);
    processMessage(*msg);
    delete msg;
  }

  return 0;
}

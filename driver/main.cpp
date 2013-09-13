#include "BeagleBone.h"
#include "PhysicalPiano.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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
    
    static Message* read(unsigned int socket);
};

Message* Message::read(unsigned int socket) {
  uint32_t length;
  if (recv(socket, &length, sizeof(length), 0) < 1) {
    return NULL;
  }

  printf("read length: %d\n", length);

  uint8_t* buffer = new uint8_t[length];
  if (recv(socket, buffer, length, 0) < 1) {
    fatal("EOF reading message");
  }

  return new Message(length, buffer);
}

static unsigned int remoteSocket;
static SPI* spi = NULL;
static PhysicalPiano* piano = NULL;

static void sendMessage(Command cmd, const uint8_t* body, uint32_t bodyLength) {
  uint32_t totalLength = bodyLength + sizeof(cmd);
  if (send(remoteSocket, &totalLength, sizeof(totalLength), 0) == -1 ||
      send(remoteSocket, &cmd, sizeof(cmd), 0) == -1 ||
      (bodyLength > 0 && send(remoteSocket, body, bodyLength, 0) == -1)) {
    fatal("error sending message");
  }
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
  printf("processing message!\n");
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
/*
int main(int argc, char** argv) {
  if (argc < 3) {
    fatal("need to specify pipe names");
  }

  FILE* inPipe = fopen(argv[1], "rb+");
  outPipe = fopen(argv[2], "wb+");

#ifndef IS_SIMULATOR
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
*/

int main(int argc, char** argv) {
  if (argc < 2) {
    fatal("need to specify socket name");
  }

#ifndef IS_SIMULATOR
  spi = new SPI(4e6);
  piano = new PhysicalPiano();
#endif

  unsigned int s = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un local;
  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, argv[1]);
  unlink(local.sun_path);
  socklen_t len = strlen(local.sun_path) + sizeof(local.sun_family) + 1;
  bind(s, (struct sockaddr*)&local, len);
  listen(s, 5);

  log("Waiting for connection...");

  struct sockaddr_un remote;
  len = sizeof(struct sockaddr_un);
  remoteSocket = accept(s, (struct sockaddr*)&remote, &len);

  log("Connected!");

  sendMessage(CMD_OK);
  while(1) {
    Message* msg = Message::read(remoteSocket);
    if (msg == NULL) {
      log("Disconnected!");
      break;
    }
    printf("got something\n");
    processMessage(*msg);
    delete msg;
  }

//  char buf[4096];
//  while(len = recv(remoteSocket, &buf, sizeof(buf), 0), len > 0) {
//    printf("[echo] %.*s\n", len, buf);
//    send(remoteSocket, &buf, len, 0);
//  }

  close(remoteSocket);

  return 0;
}

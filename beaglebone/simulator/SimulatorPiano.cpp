#include "SimulatorPiano.h"
#include "Util.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

SimulatorPiano::SimulatorPiano(PianoDelegate* delegate) : Piano(delegate) {
  read_buffer_pos = 0;
}

static bool isKeypressAvailable() {
  struct timeval tv;
  fd_set fds;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  FD_ZERO(&fds);
  FD_SET(STDIN_FILENO, &fds);
  select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &fds);
}

bool SimulatorPiano::scan() {
  bool changes = false;
  while (isKeypressAvailable()) {
    char c = getchar();
    if (read_buffer_pos == SimulatorPiano::READ_BUFFER_SIZE) {
      Util::fatal("ran out of read buffer space");
    }

    if (c == '\n') {
      read_buffer[read_buffer_pos] = '\0';
      if (this->processMessage(read_buffer)) {
        changes = true;
      }
      read_buffer_pos = 0;
    } else {
      read_buffer[read_buffer_pos++] = c;
    }
  }
  
  delegate->onPassFinished(changes);

  return changes;
}

bool SimulatorPiano::processMessage(const char* msg) {
  int key;
  if (sscanf(msg, "KEY_DOWN:%d", &key) > 0) {
    return this->setKeyValue(key, true);
  } else if (sscanf(msg, "KEY_UP:%d", &key) > 0) {
    return this->setKeyValue(key, false);
  } else {
    fprintf(stderr, "Unrecognized msg: [%s]\n", msg);
    return false;
  }
}

bool SimulatorPiano::setKeyValue(Key key, bool value) {
  if (key >= Piano::NUM_KEYS) {
    return false;
  } else {
    if (key_values[key] != value) {
      key_values[key] = value;

      if (value) {
        delegate->onKeyDown(key);
      } else {
        delegate->onKeyUp(key);
      }

      return true;
    } else {
      return false;
    }
  }
}

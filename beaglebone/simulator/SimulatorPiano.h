#include "Piano.h"

class SimulatorPiano : public Piano {
  public:
    SimulatorPiano(PianoDelegate* delegate);
    virtual bool scan();

  private:
    static const int READ_BUFFER_SIZE = 256;
    char read_buffer[READ_BUFFER_SIZE];
    int read_buffer_pos;

    bool processMessage(const char* msg);
    bool setKeyValue(Key key, bool value);
};


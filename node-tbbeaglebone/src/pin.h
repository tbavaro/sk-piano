#ifndef __INCLUDED_PIN_H
#define __INCLUDED_PIN_H

#include <string>

class Pin {
  public:
    enum Mode {
      NONE,
      INPUT,
      OUTPUT
    };

    static const std::string& modeName(Mode mode);

    Pin(int number);
    virtual ~Pin();

    virtual void close();
    virtual void setMode(Mode mode);
    virtual bool read() = 0;
    virtual void write(bool value) = 0;

    std::string toString() const;

  protected:
    const int number;
    Mode getMode() const { return mode; }

  private:
    Mode mode;
};

#endif

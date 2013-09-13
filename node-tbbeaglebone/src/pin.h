#ifndef __INCLUDED_PIN_H
#define __INCLUDED_PIN_H

#include <stdio.h>
#include <string>

class Pin {
  public:
    enum Mode {
      NONE,
      INPUT,
      OUTPUT
    };

    Pin(const std::string& name, int number);
    ~Pin();

    void close();
    void setMode(Mode mode);
    bool read();
    void write(bool value);

    const std::string& getName() const;

  private:
    std::string name;
    int number;
    Mode mode;
    std::string gpioPath;
    std::string valueFilename;
    FILE* valueFile;

    FILE* getOrOpenValueFile();
};

#endif

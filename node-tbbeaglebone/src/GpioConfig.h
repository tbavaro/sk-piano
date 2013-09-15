#ifndef __INCLUDED_GPIO_CONFIG_H
#define __INCLUDED_GPIO_CONFIG_H

class GpioConfig {
  public:
    static const GpioConfig* userLed(int i);
    static const GpioConfig* lookup(int header, int pin);

    const int bank;
    const int pin;
    const int exportId;
    const char* const name;

//  private:
    GpioConfig(int bank, int pin, const char* name);
};

#endif

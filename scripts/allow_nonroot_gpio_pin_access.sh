#!/bin/sh -e

# sets permissions so non-root users can manipulate the given pin
# usage: allow_nonroot_gpio_pin_access.sh <pinNumber> <pinNumber> ...

for pin in $*; do
  if [[ ! $pin =~ ^[1-9][0-9]*$ ]]; then
    echo Invalid pin: "$pin"
    exit 1
  fi

  pinRootDir="/sys/class/gpio/gpio${pin}"
  chmod 666 "${pinRootDir}/direction" "${pinRootDir}/value"
done

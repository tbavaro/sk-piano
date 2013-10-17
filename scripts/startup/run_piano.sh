#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PIANO_ROOT=${SCRIPT_DIR}/../..

cd ${SCRIPT_DIR}
./export_gpio_pins.sh
./spidev_init.sh

cd ${PIANO_ROOT}
while [ true ]
do
  echo Starting piano...

  # don't log anything (or log to ramdisk maybe? logging to SD card is bad)
  make run 2>&1 > /dev/null
 
  echo "Process exited (crashed?); waiting to restart..."
  # sleep so we don't thrash if there's a problem starting
  sleep 1
done


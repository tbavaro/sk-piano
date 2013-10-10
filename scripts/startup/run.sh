#!/bin/sh

PIANO_ROOT=/home/piano/git/piano

cd ${PIANO_ROOT}/scripts/startup
./export_gpio_pins.sh
./spidev_init.sh

cd ${PIANO_ROOT}
while [ true ]
do
  make run
  
  # sleep so we don't thrash if there's a problem starting
  sleep 1
done


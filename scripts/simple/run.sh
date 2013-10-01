#!/bin/sh

/home/piano/git/piano/scripts/simple/export_gpio_pins.sh
/home/piano/git/piano/scripts/simple/spidev_init.sh

cd /home/piano/git/piano/piano
while [ true ]
do
  make run
  
  # sleep so we don't thrash if there's a problem starting
  sleep 1
done


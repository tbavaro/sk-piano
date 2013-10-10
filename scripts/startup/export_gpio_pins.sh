#!/bin/sh

INPUT_PINS=(47 23 36 62 61 33 22 27 32 65 63 37)
OUTPUT_PINS=(44 26 46 66 69 45 68 67)

for pin in ${INPUT_PINS[@]}; do
  echo $pin > /sys/class/gpio/export
  echo in > /sys/class/gpio/gpio${pin}/direction
done

for pin in ${OUTPUT_PINS[@]}; do
  echo $pin > /sys/class/gpio/export
  echo out > /sys/class/gpio/gpio${pin}/direction
done


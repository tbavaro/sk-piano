#!/bin/sh

INPUT_PINS=(45 44 23 26 47 46 27 65 86 88 87 89)
OUTPUT_PINS=(76 77 74 75 72 73 70 71)

for pin in ${INPUT_PINS[@]}; do
  echo $pin > /sys/class/gpio/export
  echo in > /sys/class/gpio/gpio${pin}/direction
done

for pin in ${OUTPUT_PINS[@]}; do
  echo $pin > /sys/class/gpio/export
  echo out > /sys/class/gpio/gpio${pin}/direction
done


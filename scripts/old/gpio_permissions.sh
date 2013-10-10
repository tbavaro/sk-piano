#!/bin/sh -e

FILES=( \
  "/sys/class/gpio/export" \
  "/sys/class/gpio/unexport" \
)

for file in ${FILES[@]}; do
  chmod 666 "$file"
done

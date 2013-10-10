#!/bin/sh -e

if [[ ! -a /dev/spidev1.0 ]]; then
  echo BB-SPIDEV0 > /sys/devices/bone_capemgr.7/slots
fi

if [[ ! -a /dev/spidev2.0 ]]; then
  echo BB-SPIDEV1 > /sys/devices/bone_capemgr.7/slots
fi

# make them accessable to all users
devices=(spidev1.0 spidev1.1 spidev2.0 spidev2.1)
for f in ${devices[@]}; do
  chmod 666 "/dev/$f"
done

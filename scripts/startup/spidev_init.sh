#!/bin/sh -e

if [[ ! -a /dev/spidev1.0 ]]; then
  echo BB-SPIDEV0 > /sys/devices/bone_capemgr.7/slots
fi

if [[ ! -a /dev/spidev2.0 ]]; then
  echo BB-SPIDEV1 > /sys/devices/bone_capemgr.7/slots
fi


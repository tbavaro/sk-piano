#!/bin/sh -e

SLOTS_FILE=/sys/devices/bone_capemgr.7/slots
if [[ ! -a ${SLOTS_FILE} ]]; then
  SLOTS_FILE=/sys/devices/bone_capemgr.9/slots
fi

if [[ ! -a /dev/spidev1.0 ]]; then
  echo BB-SPIDEV0 > ${SLOTS_FILE}
fi

if [[ ! -a /dev/spidev2.0 ]]; then
  echo BB-SPIDEV1 > ${SLOTS_FILE}
fi


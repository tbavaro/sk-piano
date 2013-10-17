#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PIANO_ROOT=${SCRIPT_DIR}/../..

cd ${PIANO_ROOT}
while [ true ]
do
  echo Starting piano simulator...

  # don't log anything (or log to ramdisk maybe? logging to SD card is bad)
  make run-sim 2>&1 > /dev/null
 
  echo "Process exited (crashed?); waiting to restart..."
  # sleep so we don't thrash if there's a problem starting
  sleep 1
done


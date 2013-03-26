#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PIANO_HOME=$SCRIPT_DIR/..

cd $PIANO_HOME/beaglebone
while [ true ]
do
  ./piano

  # sleep so we don't thrash if there's a problem starting
  sleep 1
done


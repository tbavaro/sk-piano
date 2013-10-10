#!/bin/sh

SD_CARD_DEVICE=/dev/rdisk7
OUTPUT_FILE=~/Desktop/piano-sdcard.img

set -e
set -x

ddrescue -b 65536 ${SD_CARD_DEVICE} ${OUTPUT_FILE}
gzip ${OUTPUT_FILE}


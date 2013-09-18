ASANA_NODE_ROOT=${CODEZ}/build/node
NODE_ROOTS=${ASANA_NODE_ROOT}/bin:/usr/local/bin
NODE_MODULE_ROOTS=${ASANA_NODE_ROOT}/lib/node_modules:/usr/local/lib/node_modules

PIANO_NATIVE_ROOT=./native
PIANO_NATIVE_OUTPUT_PATH=${PIANO_NATIVE_ROOT}/build/Release

PATH:=${PATH}:${NODE_ROOTS}

NODE_PATH:=./base:./test:./visualizers:${PIANO_NATIVE_OUTPUT_PATH}:${NODE_PATH}:${NODE_MODULE_ROOTS}

export PATH
export NODE_PATH

.PHONY: all native clean run test

all: native

native:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp configure build )

clean:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp clean )

run:
	coffee beaglebone/RealPiano

test:
	nodeunit test/AllTests.js

#xcxc
coffee:
	coffee

#xcxc
node:
	node

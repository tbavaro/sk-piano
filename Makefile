PIANO_NATIVE_ROOT=./native
PIANO_NATIVE_OUTPUT_PATH=${PIANO_NATIVE_ROOT}/build/Release

NODE_PATH:=./base:./test:./visualizers:${PIANO_NATIVE_OUTPUT_PATH}:${NODE_PATH}
export NODE_PATH

.PHONY: all native clean run test

all: native node_modules

native:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp configure build )

clean:
	rm -rf node_modules
	( cd ${PIANO_NATIVE_ROOT} && node-gyp clean )

run: node_modules
	coffee beaglebone/RealPiano

sim: node_modules
	coffee --compile .

test: node_modules
	./node_modules/nodeunit/bin/nodeunit test/AllTests.js

node_modules:
	npm install

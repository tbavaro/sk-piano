PIANO_NATIVE_ROOT=./src/native
PIANO_NATIVE_OUTPUT_PATH=${PIANO_NATIVE_ROOT}/build/Release

NODE_PATH:=./src:./src/base:./src/visualizers:${PIANO_NATIVE_OUTPUT_PATH}:${NODE_PATH}
export NODE_PATH

.PHONY: all native clean run test

all: native node_modules

native:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp configure build )

clean:
	rm -rf node_modules
	rm -f sim_www/sim_generated.js
	( cd ${PIANO_NATIVE_ROOT} && node-gyp clean )

run: node_modules
	coffee src/beaglebone/RealPiano

sim: node_modules
	ln -sf `pwd`/src/sim/AssertStub.coffee ./node_modules/assert.coffee
	./node_modules/polvo/bin/polvo -r

test: node_modules
	./node_modules/nodeunit/bin/nodeunit src/test/AllTests.js

node_modules:
	npm install

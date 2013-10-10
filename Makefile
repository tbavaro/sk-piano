PIANO_NATIVE_ROOT=./src/native
PIANO_NATIVE_OUTPUT_PATH=${PIANO_NATIVE_ROOT}/build/Release

NODE_PATH:=./src:./src/base:./src/visualizers:${PIANO_NATIVE_OUTPUT_PATH}:${NODE_PATH}
export NODE_PATH

ASSERT_STUB_REAL_PATH=src/sim/AssertStub.coffee
ASSERT_STUB_SYMLINK_PATH=node_modules/assert.coffee

COMPILED_SHADERS_FILENAME=CompiledShaders.js
COMPILED_SHADERS_ROOT=src/sim/webgl
COMPILED_SHADERS_PATH=${COMPILED_SHADERS_ROOT}/${COMPILED_SHADERS_FILENAME}

SIM_GENERATED_JS=sim_www/sim_generated.js

.PHONY: all native clean run sim sim-auto test ${SIM_GENERATED_JS}

all: native node_modules

native:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp configure build )

clean:
	rm -rf node_modules
	rm -f ${SIM_GENERATED_JS}
	rm -f ${COMPILED_SHADERS_PATH}
	( cd ${PIANO_NATIVE_ROOT} && node-gyp clean )

run: node_modules
	coffee src/beaglebone/RealPiano.coffee

${ASSERT_STUB_SYMLINK_PATH}: node_modules ${ASSERT_STUB_REAL_PATH}
	cp ${ASSERT_STUB_REAL_PATH} ${ASSERT_STUB_SYMLINK_PATH}

${COMPILED_SHADERS_PATH}: ${COMPILED_SHADERS_ROOT}/*.vert ${COMPILED_SHADERS_ROOT}/*.frag
	( cd ${COMPILED_SHADERS_ROOT} && ./compile_shaders.rb > ${COMPILED_SHADERS_FILENAME} )

${SIM_GENERATED_JS}:
	./node_modules/polvo/bin/polvo -c # -r

sim: node_modules ${ASSERT_STUB_SYMLINK_PATH} ${COMPILED_SHADERS_PATH} ${SIM_GENERATED_JS}

sim-auto:
	coffee tools/AutoMakeSim.coffee

test: node_modules
	./node_modules/nodeunit/bin/nodeunit src/test/AllTests.js

node_modules:
	npm install

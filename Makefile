PIANO_NATIVE_ROOT=./src/native
PIANO_NATIVE_OUTPUT_PATH=${PIANO_NATIVE_ROOT}/build/Release

NODE_PATH:=./src:./src/base:./src/visualizers:${PIANO_NATIVE_OUTPUT_PATH}:${NODE_PATH}
export NODE_PATH

ASSERT_STUB_REAL_PATH=src/sim/AssertStub.coffee
ASSERT_STUB_SYMLINK_PATH=node_modules/assert.coffee

COMPILED_SHADERS_FILENAME=CompiledShaders.js
COMPILED_SHADERS_ROOT=src/sim/webgl
COMPILED_SHADERS_PATH=${COMPILED_SHADERS_ROOT}/${COMPILED_SHADERS_FILENAME}

SIM_WWW_ROOT=sim_www

SIM_GENERATED_JS=${SIM_WWW_ROOT}/sim_generated.js
SIM_GENERATED_CSS=${SIM_WWW_ROOT}/sim.css
SIM_SCSS=src/sim/sim.scss
SIM_GENERATED_COFFEEDOC_ROOT=${SIM_WWW_ROOT}/docs

COFFEEDOC_INPUTS_ROOT=src/lib

.PHONY: all native clean run sim sim-auto test sim-force

all: native node_modules sim docs

native:
	( cd ${PIANO_NATIVE_ROOT} && node-gyp configure build )

clean:
	rm -rf node_modules
	rm -f ${SIM_GENERATED_JS}
	rm -f ${COMPILED_SHADERS_PATH}
	rm -rf ${SIM_GENERATED_COFFEEDOT_ROOT}/*
	( cd ${PIANO_NATIVE_ROOT} && node-gyp clean )

run: node_modules
	coffee src/beaglebone/RealPiano.coffee

run-coffee: all
	coffee

${ASSERT_STUB_SYMLINK_PATH}: node_modules ${ASSERT_STUB_REAL_PATH}
	cp ${ASSERT_STUB_REAL_PATH} ${ASSERT_STUB_SYMLINK_PATH}

${COMPILED_SHADERS_PATH}: ${COMPILED_SHADERS_ROOT}/*.vert ${COMPILED_SHADERS_ROOT}/*.frag
	( cd ${COMPILED_SHADERS_ROOT} && ./compile_shaders.rb > ${COMPILED_SHADERS_FILENAME} )

sim-force: ${SIM_GENERATED_CSS}
	./node_modules/polvo/bin/polvo -r

${SIM_GENERATED_JS}:
	make sim-force

${SIM_GENERATED_CSS}: ${SIM_SCSS}
	sass ${SIM_SCSS} ${SIM_GENERATED_CSS}

sim: node_modules ${ASSERT_STUB_SYMLINK_PATH} ${COMPILED_SHADERS_PATH} ${SIM_GENERATED_JS} ${SIM_GENERATED_CSS}

sim-auto:
	coffee tools/AutoMakeSim.coffee

run-sim: sim
	coffee -w src/simws/SimulatorWebServer.coffee

test: node_modules
	./node_modules/nodeunit/bin/nodeunit src/test/AllTests.js

node_modules:
	npm install

docs: node_modules
	PWD=`pwd`
	( cd ${COFFEEDOC_INPUTS_ROOT} && ${PWD}/node_modules/coffeedoc/bin/coffeedoc \
	    --output ${PWD}/${SIM_GENERATED_COFFEEDOC_ROOT} \
	    --hide-private . )
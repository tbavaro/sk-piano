NODE_ROOTS=~/Workspaces/asana/build/node/bin:/usr/local/bin
BUILD_DIR=./build/Release

.PHONY: all test

all: binding.gyp
	# hack to get make to work from inside intellij on my machine
	PATH=${PATH}:${NODE_ROOTS} node-gyp configure build

clean:
	PATH=${PATH}:${NODE_ROOTS} node-gyp clean

test:
	PATH=${PATH}:${NODE_ROOTS} \
	NODE_PATH=${NODE_PATH}:${BUILD_DIR} \
	  coffee test/node_spi_test.coffee

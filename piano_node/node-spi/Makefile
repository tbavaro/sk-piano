.PHONY: all test

all: binding.gyp spi.cpp
	node-gyp configure build

test:
	node spi-test.js

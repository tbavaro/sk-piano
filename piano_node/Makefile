ASANA_NODE_ROOT=~/Workspaces/asana/build/node
NODE_ROOTS=${ASANA_NODE_ROOT}/bin:/usr/local/bin
NODE_MODULE_ROOTS=${ASANA_NODE_ROOT}/lib/node_modules:/usr/local/lib/node_modules

.PHONY: test

test:
	PATH=${PATH}:${NODE_ROOTS} \
	NODE_PATH=${NODE_PATH}:${NODE_MODULE_ROOTS} \
    nodeunit AllTests.js


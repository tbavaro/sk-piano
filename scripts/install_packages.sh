#!/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

opkg install \
  apache2 \
  git \
  ruby

cd "${SCRIPT_DIR}/.."

su -c "npm install" piano

#!/bin/sh

set -e

mkdir -p build
cd build
qmake ..
make

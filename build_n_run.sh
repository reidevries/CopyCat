#!/bin/sh
./generate_src_cmakelists.sh
cmake --build ./build
./build/copycat $1

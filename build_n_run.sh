#!/bin/sh
cmake --build ./build
./build/copycat $1

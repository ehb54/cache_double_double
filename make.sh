#!/bin/bash -x

g++ --std=gnu++17 -O2 -o build -lm build.cpp fcache.cpp
./build
g++ --std=gnu++17 -O2 -o example example.cpp fcache_use.cpp

#!/bin/bash
set -e

mkdir -p bin

time clang++ -std=c++20 -Wall -Wextra -Wpedantic -O3 \
    src/main.cpp \
    src/cube/*.cpp \
    src/solver/*.cpp \
    src/test/*.cpp \
    -o bin/cube

echo "Build successful!"

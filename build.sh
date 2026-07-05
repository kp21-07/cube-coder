#!/bin/bash
set -e

mkdir -p bin

time clang++ -std=c++20 -Wall -Wextra -Wpedantic -O3 *.cpp -o bin/cube

echo "Build successful!"

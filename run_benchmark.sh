#!/bin/bash
set -e

# Ensure we are in project root (assuming script is run from project root)

mkdir -p bin

if [ ! -d assets ]; then
	./gen_pdb.sh
fi

echo "Compiling benchmark..."
clang++ -std=c++20 -Wall -Wextra -Wpedantic -O3 -Isrc \
    benchmark/benchmark.cpp \
    src/cube/*.cpp \
    src/solver/*.cpp \
    src/solver/pdb/pdb.cpp \
    src/test/*.cpp \
    -o bin/benchmark

echo "Running benchmark..."
bin/benchmark

#!/bin/bash
set -e

mkdir -p bin
mkdir -p assets

# Compile the generator, including pdb.cpp
time clang++ -std=c++20 -Wall -Wextra -Wpedantic -O3 \
    src/solver/pdb/create_pdb.cpp \
    src/solver/pdb/pdb.cpp \
    src/cube/*.cpp \
    -o bin/create_pdb

echo "Build successful!"
echo "Running the executable to generate all PDBs..."

# Run for all three PDBs
./bin/create_pdb corner
./bin/create_pdb edge1
./bin/create_pdb edge2

echo "Successfully generated all PDB tables in the assets/ directory!"

rm bin/create_pdb

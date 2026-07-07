# Cube Coder: A 3x3 Rubik's Cube Solver

A fun hobby project exploring different methods to find an optimal and fast way to solve a Rubik's Cube programmatically.

## Current Performance
The solver currently uses IDA* (Iterative Deepening A*) with a heuristic, and is actively being improved to handle deeper scrambles more efficiently. Currently, the best achieved result is solving a **9-move scramble in 22.71 seconds** (Using the Combined Heuristic).

## Project Structure
The project is built in C++ and organized as follows:

```text
cube-coder/
├── bin/          # Compiled executable binaries
├── src/          # Main source code
│   ├── cube/     # Core Rubik's Cube state representation and move logic
│   ├── solver/   # Search algorithms (IDDFS, IDA*, heuristics)
│   ├── test/     # Automated test framework for validating cube logic
│   ├── utils/    # Helper functions and utilities
│   └── main.cpp  # Application entry point
├── build.sh      # Bash script to quickly compile the project
```

## Approaches Explored
- Iterative Deepening Depth-First Search (IDDFS)
- Iterative Deepening A* (IDA*)

## Approaches to be Explored
- Pattern Databases
- Bitboard Representation
- Optimal Pruning (Pruning Tables)
- Distance from solved Heuristic
- A form of Biderectional search (Probably not on IDA* though)A

## How to Build and Run
This project requires a standard C++ compiler (like `g++` or `clang++`).
To compile and run the solver, simply execute the included build script:
```bash
./build.sh
./bin/cube
```
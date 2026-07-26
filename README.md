# Cube Coder: A 3x3 Rubik's Cube Solver

A fun hobby project exploring different methods to find an optimal and fast way to solve a Rubik's Cube programmatically.

## Current Performance
The solver currently uses IDA* (Iterative Deepening A*) with a heuristic, and is actively being improved to handle deeper scrambles more efficiently. The achieved results for various scramble lengths are as follows:

| **Scramble Length** | **Avg Time (sec)** | **Min Time (sec)** | **Max Time (sec)** |
| :---: | :---: |
| 12-move scramble |  0.147 | 0.014 | 0.404 |
| 13-move scramble |  1.832 | 0.237 | 5.047 |
| 14-move scramble |  16.800 | 2.007 | 43.696 |
| 15-move scramble | 198.403 | 31.753 | 573.506 |

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

## License
This project is open-source and available under the [MIT License](LICENSE).

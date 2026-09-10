# Cube Coder: A 3x3 Rubik's Cube Solver

A fun hobby project exploring programmatic methods for solving a Rubik's Cube, progressing from single-phase IDA* search to an efficient Kociemba Two-Phase algorithm.

## Performance Evolution

### Initial Single-Phase IDA* (Historical)
The initial solver relied on single-phase IDA* search with basic heuristics. While efficient for short scrambles, the search tree exploded exponentially as depth increased:

| **Scramble Length** | **Avg Time (sec)** | **Min Time (sec)** | **Max Time (sec)** |
| :---: | :---: | :---: | :---: |
| 12-move scramble | 0.133 | 0.012 | 0.469 |
| 13-move scramble | 1.378 | 0.155 | 3.648 |
| 14-move scramble | 14.361 | 0.089 | 45.729 |
| 15-move scramble | 155.688 | 27.004 | 399.014 |

---

### Kociemba Two-Phase Algorithm (Current)
By transitioning to Kociemba's Two-Phase Algorithm using precomputed coordinate move tables and nibble-packed BFS pruning tables (~1M entries per Phase 1 table), the solver now resolves full **20+ move random scrambles** in sub-second speeds.

| **Scramble Length** | **Avg Solution Length** | **Avg Time** |
| :---: | :---: | :---: | 
| 25-move scramble | ~22–25 moves | ~ 0.088s | 

---

## Technical Architecture & Implementation Details

- **Phase 1 Reduction ($G_0 \rightarrow G_1$):** Reduces the cube to the $G_1 = \langle U, D, R^2, L^2, F^2, B^2 \rangle$ subgroup by orienting all edges, orienting all corners, and placing middle-layer edges into the E-slice.
- **Phase 2 Subgroup Search ($G_1 \rightarrow G_2$):** Solves corner/edge permutations exclusively using Phase 2 moves (`U`, `D`, `R2`, `L2`, `F2`, `B2`).
- **Pruning Tables:** Memory-efficient 4-bit nibble tables generated via BFS queue initialization:
  - `slice_twist_prune` ($495 \times 2187$)
  - `slice_flip_prune` ($495 \times 2048$)
  - `corner_slice_prune` ($40,320 \times 24$)
  - `ud_edges_slice_prune` ($40,320 \times 24$)

---

## Project Structure

```text
cube-coder/
├── bin/          # Compiled executable binaries
├── src/          # Main source code
│   ├── cube/     # Core Rubik's Cube state representation and coordinate moves
│   ├── solver/   # Kociemba solver (main branch), IDA* search, and pruning table generation (pdb branch)
│   ├── test/     # Test functions to verify the solver
│   ├── utils/    # Helper functions
│   └── main.cpp  # Application entry point
├── build.sh      # Bash script to compile the project

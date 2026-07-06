#pragma once

#include "../cube/cube.h"
#include "../utils/timer.h"
#include "heuristic.h"

const Move ALL_MOVES[] = {
	Move::U, Move::U_Prime, Move::U2,
	Move::R, Move::R_Prime, Move::R2,
	Move::F, Move::F_Prime, Move::F2,
	Move::D, Move::D_Prime, Move::D2,
	Move::L, Move::L_Prime, Move::L2,
	Move::B, Move::B_Prime, Move::B2,
};

int move_face(Move m);
int generate_moves(Move last_move, Move second_to_last, Move out[18]);
int generate_moves(Cube& cube, const Heuristic& h, Move last_move, Move second_to_last, Move out[18], int child_h[18]);

struct SearchResult {
	bool solved = false;
	vector<Move> solution;

	u64 states_visited     = 0;  // every node the DFS touches, including leaves and the root
	u64 nodes_expanded     = 0;  // interior nodes whose children were actually generated
	u64 children_generated = 0;  // total move-children tried across all expansions
	int depth              = 0;  // deepest iteration reached (equals solution length when solved)

	double elapsed_ms    = 0.0;  // wall-clock duration of the whole solve [milliseconds]
	double states_per_sec = 0.0; // states_visited / elapsed_sec  — raw DFS visit rate
	double mnodes_per_sec = 0.0; // nodes_expanded / elapsed_sec / 1e6  — expansion rate [Mnodes/s]
};

class IDDFS_Solver {
public:
	bool solve(Cube& cube, int depth_limit);

	SearchResult res;

private:
	bool dfs(Cube& cube,
	         int depth_rem,
	         Move last_move,
	         Move second_to_last,
	         vector<Move>& sol);
};

class IDA_Solver {
public:
	IDA_Solver(const Heuristic& h) : heuristic(h) {}

	bool solve(Cube& cube);

	SearchResult res;

private:
	const Heuristic& heuristic;
	int a_star(Cube& cube,
	           int g,
	           int bound,
	           Move last_move,
	           Move second_to_last,
	           vector<Move>& sol);
};

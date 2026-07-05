#pragma once

#include "cube.h"

const Move ALL_MOVES[] = {
	Move::U, Move::U_Prime, Move::U2,
	Move::R, Move::R_Prime, Move::R2,
	Move::F, Move::F_Prime, Move::F2,
	Move::D, Move::D_Prime, Move::D2,
	Move::L, Move::L_Prime, Move::L2,
	Move::B, Move::B_Prime, Move::B2,
};

struct SearchResult {
	bool solved = false;
	vector<Move> solution;
	uint64_t nodes_expanded = 0;
	int depth = 0;
};

class IDDFS_Solver {
public:
	bool solve(Cube& cube, int depth_limit);
	SearchResult res;

private:
	bool dfs(Cube& cube,
					 int depth_rem,
					 Move last_move,
					 vector<Move>& sol);
};

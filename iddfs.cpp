#include "solve.h"

static int move_face(Move m)
{
	switch (m)
	{
		case U:
		case U2:
		case U_Prime:
			return 0;

		case R:
		case R2:
		case R_Prime:
			return 1;

		case F:
		case F2:
		case F_Prime:
			return 2;

		case D:
		case D2:
		case D_Prime:
			return 3;

		case L:
		case L2:
		case L_Prime:
			return 4;

		case B:
		case B2:
		case B_Prime:
			return 5;

		default:
			return -1;
	}
}

static int generate_moves(Move m, Move out[18])
{
	int count = 0;
	int last_face = move_face(m);
	for (Move m : ALL_MOVES) {
		int face = move_face(m);
		if (last_face >= 0 && (face == last_face || (face+3)%6 == last_face))
				continue;
		out[count++] = m;
	}
	return count;
}

bool IDDFS_Solver::dfs(Cube& cube, int depth_rem, Move last_move, vector<Move>& sol)
{
	res.nodes_expanded++;

	if (cube.is_solved())
		return true;

	if (depth_rem == 0)
		return false;

	Move moves[18];
	int n = generate_moves(last_move, moves);

	for (int i = 0; i < n; i++) {
		Move m = moves[i];

		cube.apply_move(m);
		sol.push_back(m);
		
		if (dfs(cube, depth_rem-1, m, sol)) 
			return true;

		sol.pop_back();
		cube.apply_move(inverse_move(m));
	}

	return false;
}

bool IDDFS_Solver::solve(Cube& cube, int depth_limit)
{
	res = SearchResult{};
	for (int depth = 0; depth <= depth_limit; depth++) {
		res.depth = depth;
		res.solution.clear();

		if (dfs(cube, depth, Move::None, res.solution)) {
			res.solved = true;
			return true;
		}
	}
	return false;
}

#include "solve.h"

bool IDDFS_Solver::dfs(Cube& cube, int depth_rem, Move last_move, Move second_to_last, vector<Move>& sol)
{
	res.states_visited++;

	if (cube.is_solved())
		return true;

	if (depth_rem == 0)
		return false;

	res.nodes_expanded++;

	Move moves[18];
	int n = generate_moves(last_move, second_to_last, moves);
	res.children_generated += n;

	for (int i = 0; i < n; i++) {
		Move m = moves[i];

		cube.apply_move(m);
		sol.push_back(m);

		if (dfs(cube, depth_rem-1, m, last_move, sol))
			return true;

		sol.pop_back();
		cube.apply_move(inverse_move(m));
	}

	return false;
}

bool IDDFS_Solver::solve(Cube& cube, int depth_limit)
{
	res = SearchResult{};
	Timer timer;

	for (int depth = 0; depth <= depth_limit; depth++) {
		res.depth = depth;
		res.solution.clear();

		if (dfs(cube, depth, Move::None, Move::None, res.solution)) {
			res.solved = true;
			break;
		}
	}

	res.elapsed_ms = timer.elapsed_ms();
	double elapsed_sec = res.elapsed_ms / 1000.0;
	res.states_per_sec  = (elapsed_sec > 0.0) ? (double)res.states_visited  / elapsed_sec       : 0.0;
	res.mnodes_per_sec  = (elapsed_sec > 0.0) ? (double)res.nodes_expanded  / elapsed_sec / 1e6 : 0.0;

	return res.solved;
}

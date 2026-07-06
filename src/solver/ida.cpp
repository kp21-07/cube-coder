#include "solve.h"

static const int FOUND = -1;
static const int INF = 1e9;

int IDA_Solver::a_star(Cube& cube, int g, int bound, Move last_move, Move second_to_last, vector<Move>& sol)
{
	res.states_visited++;

	CubeState state = cube.extract_state();
	int h = heuristic.evaluate(state);
	int f = g + h;

	if (f > bound)
		return f;

	if (h == 0 && cube.is_solved()) {
		res.depth = g;
		return FOUND;
	}

	res.nodes_expanded++;

	Move moves[18];
	int n = generate_moves(last_move, second_to_last, moves);
	res.children_generated += n;

	int minimum = INF;

	for (int i = 0; i < n; i++) {
		Move m = moves[i];

		cube.apply_move(m);
		sol.push_back(m);

		int t = a_star(cube, g+1, bound, m, last_move, sol);

		if (t == FOUND)
			return FOUND;

		if (t < minimum)
			minimum = t;

		sol.pop_back();
		cube.apply_move(inverse_move(m));
	}

	return minimum;
}

bool IDA_Solver::solve(Cube& cube)
{
	res = SearchResult{};
	Timer timer;

	CubeState root_state = cube.extract_state();
	int bound = heuristic.evaluate(root_state);

	for(;;) {
		res.solution.clear();

		int t = a_star(cube, 0, bound, Move::None, Move::None, res.solution);

		if (t == FOUND) {
			res.solved = true;
			break;
		}

		if (t == INF)
			break;

		bound = t;
	}

	res.elapsed_ms = timer.elapsed_ms();
	double elapsed_sec = res.elapsed_ms / 1000.0;
	res.states_per_sec  = (elapsed_sec > 0.0) ? (double)res.states_visited  / elapsed_sec       : 0.0;
	res.mnodes_per_sec  = (elapsed_sec > 0.0) ? (double)res.nodes_expanded  / elapsed_sec / 1e6 : 0.0;

	return res.solved;
}

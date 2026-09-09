#include "solve.h"

/*
╔═══════════════════════════════════════════════════════════╗
║                   PHASE 1 IDA* SEARCH                     ║
╚═══════════════════════════════════════════════════════════╝
*/

bool KociembaSolver::phase1_search(SearchState state, const Cube& current_cube, int g, int bound, Move last_m) {
	int h = phase1_h(state.twist, state.flip, state.slice);
	if (g + h > bound) return false;


	if (state.twist == 0 && state.flip == 0 && state.slice == 0) {
		start_phase2(current_cube);
		if (found_solution) return true;
	}

	for (int m_idx = 0; m_idx < N_MOVES; m_idx++) {
		Move m = (Move)m_idx;
		if (is_redundant_move(m, last_m)) continue;

		SearchState next_state = state;
		next_state.twist = twist_move[state.twist][m_idx];
		next_state.flip  = flip_move[state.flip][m_idx];
		next_state.slice = slice_move[state.slice][m_idx];

		Cube next_cube = current_cube;
		next_cube.apply_move(m);

		phase1_moves.push_back(m);
		if (phase1_search(next_state, next_cube, g + 1, bound, m)) return true;
		phase1_moves.pop_back();
	}

	return false;
}

/*
╔═══════════════════════════════════════════════════════════╗
║                   PHASE 2 IDA* SEARCH                     ║
╚═══════════════════════════════════════════════════════════╝
*/

bool KociembaSolver::phase2_search(SearchState state, int g, int bound, Move last_m) {
	int h = phase2_h(state.corner, state.ud_edges, state.slice_perm);
	if (g + h > bound) return false;

	if (state.corner == 0 && state.ud_edges == 0 && state.slice_perm == 0) {
		solution = phase1_moves;
		return true; 
	}

	for (int i = 0; i < 10; i++) {
		Move m = PHASE2_MOVES[i];
		if (is_redundant_move(m, last_m)) continue;

		SearchState next_state = state;
		next_state.corner     = corner_perm_move[state.corner][(int)m];
		next_state.ud_edges   = ud_edges_move[state.ud_edges][(int)m];
		next_state.slice_perm = slice_perm_move[state.slice_perm][(int)m];

		phase1_moves.push_back(m);
		if (phase2_search(next_state, g + 1, bound, m)) return true;
		phase1_moves.pop_back();
	}

	return false;
}

void KociembaSolver::start_phase2(const Cube& cube) {
	SearchState p2_state;
	p2_state.corner     = cube.get_corner_perm();
	p2_state.ud_edges   = cube.get_ud_edges_perm();
	p2_state.slice_perm = cube.get_slice_perm();

	int p2_h = phase2_h(p2_state.corner, p2_state.ud_edges, p2_state.slice_perm);

	for (int bound = p2_h; bound <= 18; bound++) {
		if (phase2_search(p2_state, 0, bound, Move::None)) {
			found_solution = true;
			break;
		}
	}
}

vector<Move> KociembaSolver::solve(Cube cube) {
	solution.clear();
	phase1_moves.clear();
	found_solution = false;

	SearchState p1_state;
	p1_state.twist = cube.get_twist();
	p1_state.flip  = cube.get_flip();
	p1_state.slice = cube.get_uds_slice();

	int p1_h = phase1_h(p1_state.twist, p1_state.flip, p1_state.slice);

	// Iterative deepening search for Phase 1
	for (int bound = p1_h; bound <= 12; bound++) {
		if (phase1_search(p1_state, cube, 0, bound, Move::None)) {
			break;
		}
	}

	return solution;
}

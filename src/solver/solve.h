#include "kociemba.h"

struct SearchState {
	// Phase 1
	u16 twist;
	u16 flip;
	u16 slice;

	// Phase 2
	u16 corner;
	u16 ud_edges;
	u8  slice_perm;
};

/*
╔═══════════════════════════════════════════════════════════════╗
║                       KOCIEMBA'S SOLVER                       ║
╚═══════════════════════════════════════════════════════════════╝
*/
class KociembaSolver {
public:
	KociembaSolver() {}
	vector<Move> solve(Cube cube);

private:
	vector<Move> solution;
	vector<Move> phase1_moves;
	bool found_solution;

	// Phase 1 Heuristic
	inline int phase1_h(u16 twist, u16 flip, u16 slice) const {
		int idx1 = slice * N_TWIST + twist;
		int idx2 = slice * N_FLIP + flip;
		int h1 = get_prune(slice_twist_prune, idx1);
		int h2 = get_prune(slice_flip_prune, idx2);
		return (h1 > h2) ? h1 : h2;
	}

	// Phase 2 Heuristic
	inline int phase2_h(u16 corner, u16 ud_edges, u8 slice_perm) const {
		int idx1 = corner * N_SLICE_PERM + slice_perm;
		int idx2 = ud_edges * N_SLICE_PERM + slice_perm;
		int h1 = get_prune(corner_slice_prune, idx1);
		int h2 = get_prune(ud_edges_slice_prune, idx2);
		return (h1 > h2) ? h1 : h2;
	}

	inline bool is_redundant_move(Move m, Move last_m) const {
		if (last_m == Move::None) return false;

		int face1 = (int)m / 3;
		int face2 = (int)last_m / 3;

		if (face1 == face2) return true;

		if ((face1 == (int)Face::Up && face2 == (int)Face::Down) ||
				(face1 == (int)Face::Left && face2 == (int)Face::Right) ||
				(face1 == (int)Face::Front && face2 == (int)Face::Back)) {
			return face1 < face2;
		}

		return false;
	}

	bool phase1_search(SearchState state, const Cube& current_cube, int g, int bound, Move last_m);

	void start_phase2(const Cube& cube);
	bool phase2_search(SearchState state, int g, int bound, Move last_m);
};

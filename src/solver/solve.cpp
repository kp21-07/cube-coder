#include "solve.h"

int move_face(Move m)
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

int generate_moves(Move last_move, Move second_to_last, Move out[18])
{
	int count = 0;
	int last_face   = move_face(last_move);
	int second_face = move_face(second_to_last);

	// Two-move pruning: if last two moves are on opposite faces they commute,
	// so a move back on second_face duplicates a branch already explored.
	bool prune_second = (second_face >= 0 && last_face >= 0 &&
	                     (last_face + 3) % 6 == second_face);

	for (Move m : ALL_MOVES) {
		int face = move_face(m);
		// 1-move: same face or opposite face as last move
		if (last_face >= 0 && (face == last_face || (face + 3) % 6 == last_face))
			continue;
		// 2-move: last two on opposite faces → prune second_face
		if (prune_second && face == second_face)
			continue;
		out[count++] = m;
	}
	return count;
}

// Move Ordering for Searches using a heuristic
int generate_moves(Cube& cube, const Heuristic& h, Move last_move, Move second_to_last, Move out[18], int child_h[18])
{
	int count = generate_moves(last_move, second_to_last, out);

	int scores[18];
	for (int i = 0; i < count; i++) {
		cube.apply_move(out[i]);
		CubeState state = cube.extract_state();   // extract once per child position
		scores[i] = h.evaluate(state);
		cube.apply_move(inverse_move(out[i]));
	}

	// Insertion sort — co-sorts out[] and scores[] together
	for (int i = 1; i < count; i++) {
		Move key_move  = out[i];
		int  key_score = scores[i];
		int j = i - 1;
		while (j >= 0 && scores[j] > key_score) {
			out[j + 1]    = out[j];
			scores[j + 1] = scores[j];
			j--;
		}
		out[j + 1]    = key_move;
		scores[j + 1] = key_score;
	}

	// Expose sorted scores as child h-values for the caller
	for (int i = 0; i < count; i++)
		child_h[i] = scores[i];

	return count;
}


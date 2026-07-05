#pragma once

#include "types.h"

enum Move {
	U, U_Prime, U2,
	R, R_Prime, R2,
	F, F_Prime, F2,
	D, D_Prime, D2,
	L, L_Prime, L2,
	B, B_Prime, B2,

	None
};

vector<Move> parse_alg(string alg);
string       to_string(vector<Move> moves);

string inverse_alg(string alg);

Move inverse_move(Move m);

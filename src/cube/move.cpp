#include "move.h"

static Move parse_move(string token)
{
	switch (token.size()) {
		case 1:
			switch (token[0]) {
				case 'U' : return Move::U;
				case 'R' : return Move::R;
				case 'F' : return Move::F;
				case 'D' : return Move::D;
				case 'L' : return Move::L;
				case 'B' : return Move::B;
			}
			break;

		case 2:
			switch (token[1]) {
				case '\'':
					switch (token[0]) {
						case 'U' : return Move::U_Prime;
						case 'R' : return Move::R_Prime;
						case 'F' : return Move::F_Prime;
						case 'D' : return Move::D_Prime;
						case 'L' : return Move::L_Prime;
						case 'B' : return Move::B_Prime;
					}
					break;

				case '2':
					switch (token[0]) {
						case 'U' : return Move::U2;
						case 'R' : return Move::R2;
						case 'F' : return Move::F2;
						case 'D' : return Move::D2;
						case 'L' : return Move::L2;
						case 'B' : return Move::B2;
					}
					break;
			}
	}

	if (token.size() > 0)
		fprintf(stderr, "parse_move: unrecognized token '%s'\n", token.data());
	else
		fprintf(stderr, "parse_move: empty token\n");
	exit(1);
}

vector<Move> parse_alg(string alg)
{
	vector<string> tokens = alg.split(' ');
	vector<Move> moves(tokens.size());

	for (int i = 0; i < (int) tokens.size(); i++) {
		moves[i] = parse_move(tokens[i]);
	}

	return moves;
}

/*
string move_to_string[] =
{
	[U]       = "U",
	[U_Prime] = "U'",
	[U2]      = "U2",

	[D]       = "D",
	[D_Prime] = "D'",
	[D2]      = "D2",

	[L]       = "L",
	[L_Prime] = "L'",
	[L2]      = "L2",

	[R]       = "R",
	[R_Prime] = "R'",
	[R2]      = "R2",

	[F]       = "F",
	[F_Prime] = "F'",
	[F2]      = "F2",

	[B]       = "B",
	[B_Prime] = "B'",
	[B2]      = "B2",


};
 */

static string move_to_str(Move move)
{
	switch (move)
	{
		case Move::U       : return "U";
		case Move::U_Prime : return "U'";
		case Move::U2      : return "U2";

		case Move::D       : return "D";
		case Move::D_Prime : return "D'";
		case Move::D2      : return "D2";

		case Move::L       : return "L";
		case Move::L_Prime : return "L'";
		case Move::L2      : return "L2";

		case Move::R       : return "R";
		case Move::R_Prime : return "R'";
		case Move::R2      : return "R2";

		case Move::F       : return "F";
		case Move::F_Prime : return "F'";
		case Move::F2      : return "F2";

		case Move::B       : return "B";
		case Move::B_Prime : return "B'";
		case Move::B2      : return "B2";

		default:
			fprintf(stderr, "move_to_str: unrecognized Move value\n");
			exit(1);
	}
}

string to_string(vector<Move> moves)
{
	string result;
	for (int i = 0; i < (int)moves.size(); i++) {
		if (i > 0) {
			char space = ' ';
			result.append(&space, 1);
		}
		string token = move_to_str(moves[i]);
		result.append(token.data(), token.size());
	}
	return result;
}

Move inverse_move(Move m)
{
	switch (m)
	{
		case Move::U:       return Move::U_Prime;
		case Move::U_Prime: return Move::U;
		case Move::U2:      return Move::U2;

		case Move::R:       return Move::R_Prime;
		case Move::R_Prime: return Move::R;
		case Move::R2:      return Move::R2;

		case Move::F:       return Move::F_Prime;
		case Move::F_Prime: return Move::F;
		case Move::F2:      return Move::F2;

		case Move::D:       return Move::D_Prime;
		case Move::D_Prime: return Move::D;
		case Move::D2:      return Move::D2;

		case Move::L:       return Move::L_Prime;
		case Move::L_Prime: return Move::L;
		case Move::L2:      return Move::L2;

		case Move::B:       return Move::B_Prime;
		case Move::B_Prime: return Move::B;
		case Move::B2:      return Move::B2;

		default:
			fprintf(stderr, "invert_move: unrecognized Move value %u\n", (u32)m);
			exit(1);
	}
}

string inverse_alg(string alg)
{
	vector<Move> moves = parse_alg(alg);

	string result;
	for (int i = (int)moves.size() - 1; i >= 0; i--) {
		if (result.size() > 0) {
			char space = ' ';
			result.append(&space, 1);
		}
		string token = move_to_str(inverse_move(moves[i]));
		result.append(token.data(), token.size());
	}

	return result;
}

#include "cube.h"
#include "move.h"
#include <stdio.h>

Cube::Cube() { reset(); }

static constexpr int face_to_index(Face face) {
	switch (face)
	{
		case Face::Up    : return 0;
		case Face::Down  : return 1;
		case Face::Front : return 2;
		case Face::Back  : return 3;
		case Face::Left  : return 4;
		case Face::Right : return 5;
	}
}
void Cube::reset()
{
	for (int f = 0; f < 6; f++)
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
					faces[f][r][c] = FACE_COLORS[f];
}

bool Cube::is_solved()
{
	for (int f = 0; f < 6; f++)
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
					if (faces[f][r][c] != FACE_COLORS[f])
						return false;

	return true;
}

static char color_to_char(Color c)
{
	switch (c)
	{
		case Color::White  : return 'W';
		case Color::Yellow : return 'Y';
		case Color::Green  : return 'G';
		case Color::Blue   : return 'B';
		case Color::Orange : return 'O';
		case Color::Red    : return 'R';
	}

	return '?';
}

void Cube::print_face_row(Face face, int row)
{
	for (int c = 0; c < 3; c++)
	{
		printf("%c ", color_to_char(faces[face_to_index(face)][row][c]));
	}
		printf(" ");
}

void Cube::print()
{
	for (int r = 0; r < 3; r++) {
		printf("       ");
		print_face_row(Face::Up, r);
		printf("\n");
	}
	printf("\n");

	for (int r = 0; r < 3; r++) {
		print_face_row(Face::Left, r);
		print_face_row(Face::Front, r);
		print_face_row(Face::Right, r);
		print_face_row(Face::Back, r);
		printf("\n");
	}
	printf("\n");

	for (int r = 0; r < 3; r++) {
		printf("       ");
		print_face_row(Face::Down, r);
		printf("\n");
	}
	printf("\n");
	printf("\n");
}

void Cube::rotate_face_cw(Face face)
{
	int f = face_to_index(face);
	Color temp;

	temp           = faces[f][0][0];
	faces[f][0][0] = faces[f][2][0];
	faces[f][2][0] = faces[f][2][2];
	faces[f][2][2] = faces[f][0][2];
	faces[f][0][2] = temp;

	temp           = faces[f][0][1];
	faces[f][0][1] = faces[f][1][0];
	faces[f][1][0] = faces[f][2][1];
	faces[f][2][1] = faces[f][1][2];
	faces[f][1][2] = temp;
}

void Cube::U(int t)
{
	int R = face_to_index(Face::Right);
	int F = face_to_index(Face::Front);
	int L = face_to_index(Face::Left);
	int B = face_to_index(Face::Back);

	while (t--) {
		rotate_face_cw(Face::Up);
		Color temp;
		for (int c = 0; c < 3; c++) {
			temp           = faces[F][0][c];
			faces[F][0][c] = faces[R][0][c]; // Front ← Right row0
			faces[R][0][c] = faces[B][0][c]; // Right ← Back  row0
			faces[B][0][c] = faces[L][0][c]; // Back  ← Left  row0
			faces[L][0][c] = temp;           // Left  ← Front row0
		}
	}
}

void Cube::D(int t)
{
	int R = face_to_index(Face::Right);
	int F = face_to_index(Face::Front);
	int L = face_to_index(Face::Left);
	int B = face_to_index(Face::Back);

	while (t--) {
		rotate_face_cw(Face::Down);
		Color temp;
		for (int c = 0; c < 3; c++) {
			temp           = faces[F][2][c];
			faces[F][2][c] = faces[L][2][c]; // Front ← Left  row2
			faces[L][2][c] = faces[B][2][c]; // Left  ← Back  row2
			faces[B][2][c] = faces[R][2][c]; // Back  ← Right row2
			faces[R][2][c] = temp;           // Right ← Front row2
		}
	}
}

void Cube::R(int t)
{
	int U = face_to_index(Face::Up);
	int F = face_to_index(Face::Front);
	int D = face_to_index(Face::Down);
	int B = face_to_index(Face::Back);

	while (t--) {
		rotate_face_cw(Face::Right);
		Color temp;
		for (int r = 0; r < 3; r++) {
			temp             = faces[U][r][2];
			faces[U][r][2]   = faces[F][r][2];   // Up    ← Front col2
			faces[F][r][2]   = faces[D][r][2];   // Front ← Down  col2
			faces[D][r][2]   = faces[B][2-r][0]; // Down  ← Back  col0 (reversed)
			faces[B][2-r][0] = temp;             // Back  ← Up    col2 (reversed)
		}
	}
}

void Cube::L(int t)
{
	int U = face_to_index(Face::Up);
	int F = face_to_index(Face::Front);
	int D = face_to_index(Face::Down);
	int B = face_to_index(Face::Back);

	while (t--) {
		rotate_face_cw(Face::Left);
		Color temp;
		for (int r = 0; r < 3; r++) {
			temp             = faces[U][r][0];
			faces[U][r][0]   = faces[B][2-r][2]; // Up    ← Back  col2 (reversed)
			faces[B][2-r][2] = faces[D][r][0];   // Back  ← Down  col0 (reversed)
			faces[D][r][0]   = faces[F][r][0];   // Down  ← Front col0
			faces[F][r][0]   = temp;             // Front ← Up    col0
		}
	}
}

void Cube::F(int t)
{
	int U = face_to_index(Face::Up);
	int R = face_to_index(Face::Right);
	int D = face_to_index(Face::Down);
	int L = face_to_index(Face::Left);

	while (t--) {
		rotate_face_cw(Face::Front);
		Color temp;
		for (int i = 0; i < 3; i++) {
			temp             = faces[U][2][i];
			faces[U][2][i]   = faces[L][2-i][2]; // Up    ← Left  (reversed)
			faces[L][2-i][2] = faces[D][0][2-i]; // Left  ← Down  (reversed)
			faces[D][0][2-i] = faces[R][i][0];   // Down  ← Right
			faces[R][i][0]   = temp;             // Right ← Up
		}
	}
}

void Cube::B(int t)
{
	int U = face_to_index(Face::Up);
	int R = face_to_index(Face::Right);
	int D = face_to_index(Face::Down);
	int L = face_to_index(Face::Left);

	while (t--) {
		rotate_face_cw(Face::Back);
		Color temp;
		for (int i = 0; i < 3; i++) {
			temp             = faces[U][0][i];
			faces[U][0][i]   = faces[R][i][2];   // Up    ← Right
			faces[R][i][2]   = faces[D][2][2-i]; // Right ← Down  (reversed)
			faces[D][2][2-i] = faces[L][2-i][0]; // Down  ← Left  (reversed)
			faces[L][2-i][0] = temp;             // Left  ← Up
		}
	}
}

void Cube::apply_move(Move m)
{
	switch (m)
	{
		case Move::U:       return U(1);
		case Move::U2:      return U(2);
		case Move::U_Prime: return U(3);

		case Move::R:       return R(1);
		case Move::R2:      return R(2);
		case Move::R_Prime: return R(3);

		case Move::F:       return F(1);
		case Move::F2:      return F(2);
		case Move::F_Prime: return F(3);

		case Move::D:       return D(1);
		case Move::D2:      return D(2);
		case Move::D_Prime: return D(3);

		case Move::L:       return L(1);
		case Move::L2:      return L(2);
		case Move::L_Prime: return L(3);

		case Move::B:       return B(1);
		case Move::B2:      return B(2);
		case Move::B_Prime: return B(3);

		default:
			fprintf(stderr, "apply_move: unrecognized Move value %u\n", (u32)m);
			exit(1);
	}
}

void Cube::apply_moves(vector<Move>& moves)
{
	for (Move move : moves) apply_move(move);
}

void Cube::apply_algorithm(string alg)
{
	vector<Move> moves = parse_alg(alg);
	apply_moves(moves);
}

vector<Move> Cube::gen_random_scramble(int length)
{
	// U=0 R=1 F=2 D=3 L=4 B=5
	static const Move move_table[6][3] = {
		{ Move::U, Move::U_Prime, Move::U2 },
		{ Move::R, Move::R_Prime, Move::R2 },
		{ Move::F, Move::F_Prime, Move::F2 },
		{ Move::D, Move::D_Prime, Move::D2 },
		{ Move::L, Move::L_Prime, Move::L2 },
		{ Move::B, Move::B_Prime, Move::B2 },
	};

	static const int opposite[6] = { 3, 4, 5, 0, 1, 2 };

	vector<Move> scramble;

	int last_face     = -1;
	int last_opp_face = -1;

	while (length--) {
		int face;
		do {
			face = rand() % 6;
		} while (face == last_face || face == last_opp_face);

		int variant = rand() % 3; // 0=CW, 1=Prime, 2=Double
		scramble.push_back(move_table[face][variant]);

		last_opp_face = opposite[face];
		last_face     = face;
	}

	return scramble;
}

const Color& Cube::at(Face face, int row, int column)
{
	return faces[face_to_index(face)][row][column];
}

bool Cube::operator==(const Cube& other) const
{
	for (int f = 0; f < 6; f++)
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				if (faces[f][r][c] != other.faces[f][r][c])
					return false;

	return true;
}

bool Cube::operator!=(const Cube& other) const
{
	return !(*this == other);
}

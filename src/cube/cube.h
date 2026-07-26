#pragma once

#include "cubie.h"

class Cube {
public:
	Cube();

	void reset();
	bool is_solved() const;
	void print() const;

	void apply_move(Move m);
	void apply_moves(const vector<Move>& moves);
	void apply_algorithm(string alg);

	vector<Move> gen_random_scramble(int length = 25);

	void scramble(int length = 25);
	void scramble(string s);

	CubeState extract_state() const;

	bool operator==(const Cube& other) const;
	bool operator!=(const Cube& other) const;

	void to_faces(Color out_faces[6][3][3]) const;

private:
	CubeState state;

	void print_face_row(Color f[6][3][3], Face face, int row) const;

	void U(int t = 1);
	void D(int t = 1);
	void L(int t = 1);
	void R(int t = 1);
	void F(int t = 1);
	void B(int t = 1);
};

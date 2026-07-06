#pragma once

#include "cubie.h"

class Cube {
public:
	Cube();

	void reset();
	bool is_solved() const;
	void print();

	void apply_move(Move m);
	void apply_moves(const vector<Move>& moves);
	void apply_algorithm(string alg);

	vector<Move> gen_random_scramble(int length = 25);

	void scramble(int length = 25);
	void scramble(string s);

	const Color& at(Face face, int row, int column) const;

	Color facelet(const Facelet& f) const;
	void set_facelet(const Facelet& f, Color c);

	CubeState extract_state() const;

	bool operator==(const Cube& other) const;
	bool operator!=(const Cube& other) const;

private:
	Color faces[6][3][3];

	void print_face_row(Face face, int row);

	void rotate_face_cw(Face face);

	void U(int t = 1);
	void D(int t = 1);
	void L(int t = 1);
	void R(int t = 1);
	void F(int t = 1);
	void B(int t = 1);

	CornerState extract_corner(Corner pos) const;
	EdgeState   extract_edge  (Edge   pos) const;
};

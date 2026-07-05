#pragma once

#include "types.h"
#include "move.h"

enum class Face {
	Up   , Down,
	Front, Back,
	Left , Right,
};

enum class Color {
	White , Yellow,
	Green , Blue,
	Orange, Red,
};

const Color FACE_COLORS[6] = {
	Color::White,   // Up
	Color::Yellow,  // Down
	Color::Green,   // Front
	Color::Blue,    // Back
	Color::Orange,  // Left
	Color::Red      // Right
};

class Cube {
public:
	Cube();

	void reset();
	bool is_solved();
	void print();

	void apply_move(Move m);
	void apply_moves(vector<Move>& moves);
	void apply_algorithm(string alg);

	vector<Move> gen_random_scramble(int length = 20);

	const Color& at(Face face, int row, int column);

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
};

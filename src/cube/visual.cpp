#include "cube.h"

const Color FACE_COLORS[6] = {
	Color::White,   // Up
	Color::Yellow,  // Down
	Color::Green,   // Front
	Color::Blue,    // Back
	Color::Orange,  // Left
	Color::Red      // Right
};

// const Face FACES[6] = {
// 	Face::Up,
// 	Face::Down,
// 	Face::Front,
// 	Face::Back,
// 	Face::Left,
// 	Face::Right,
// };

struct Facelet {
    Face face;
    u8   row;
    u8   col;
};

struct CornerInfo {
    Corner   piece;
    Facelet  facelets[3];   // as‑solved order
    Color    solved[3];
};

struct EdgeInfo {
    Edge     piece;
    Facelet  facelets[2];
    Color    solved[2];
};

// Geometry of the cube – copied verbatim from the original cubie.h.
extern const CornerInfo CORNERS[8];
extern const EdgeInfo   EDGES[12];

// Helper to map Face enum to index (same as in cube.cpp)
static constexpr int face_to_index(Face f) {
    switch (f) {
        case Face::Up:    return 0;
        case Face::Down:  return 1;
        case Face::Front: return 2;
        case Face::Back:  return 3;
        case Face::Left:  return 4;
        case Face::Right: return 5;
    }
    return 0;
}

const CornerInfo CORNERS[8] = {
    // UFR
    { Corner::UFR, { {Face::Up, 2, 2}, {Face::Front, 0, 2}, {Face::Right, 0, 0} },
      { Color::White, Color::Green, Color::Red } },
    // URB
    { Corner::URB, { {Face::Up, 0, 2}, {Face::Right, 0, 2}, {Face::Back, 0, 0} },
      { Color::White, Color::Red, Color::Blue } },
    // UBL
    { Corner::UBL, { {Face::Up, 0, 0}, {Face::Back, 0, 2}, {Face::Left, 0, 0} },
      { Color::White, Color::Blue, Color::Orange } },
    // ULF
    { Corner::ULF, { {Face::Up, 2, 0}, {Face::Left, 0, 2}, {Face::Front, 0, 0} },
      { Color::White, Color::Orange, Color::Green } },
    // DFR
    { Corner::DFR, { {Face::Down, 0, 2}, {Face::Front, 2, 2}, {Face::Right, 2, 0} },
      { Color::Yellow, Color::Green, Color::Red } },
    // DRB
    { Corner::DRB, { {Face::Down, 2, 2}, {Face::Right, 2, 2}, {Face::Back, 2, 0} },
      { Color::Yellow, Color::Red, Color::Blue } },
    // DBL
    { Corner::DBL, { {Face::Down, 2, 0}, {Face::Back, 2, 2}, {Face::Left, 2, 0} },
      { Color::Yellow, Color::Blue, Color::Orange } },
    // DLF
    { Corner::DLF, { {Face::Down, 0, 0}, {Face::Left, 2, 2}, {Face::Front, 2, 0} },
      { Color::Yellow, Color::Orange, Color::Green } }
};

const EdgeInfo EDGES[12] = {
    // UF
    { Edge::UF, { {Face::Up, 2, 1}, {Face::Front, 0, 1} },
      { Color::White, Color::Green }},
    // UR
    { Edge::UR, { {Face::Up, 1, 2}, {Face::Right, 0, 1} },
      { Color::White, Color::Red }},
    // UB
    { Edge::UB, { {Face::Up, 0, 1}, {Face::Back, 0, 1} },
      { Color::White, Color::Blue }},
    // UL
    { Edge::UL, { {Face::Up, 1, 0}, {Face::Left, 0, 1} },
      { Color::White, Color::Orange }},
    // FR
    { Edge::FR, { {Face::Front, 1, 2}, {Face::Right, 1, 0} },
      { Color::Green, Color::Red }},
    // BR
    { Edge::BR, { {Face::Back, 1, 0}, {Face::Right, 1, 2} },
      { Color::Blue, Color::Red }},
    // BL
    { Edge::BL, { {Face::Back, 1, 2}, {Face::Left, 1, 0} },
      { Color::Blue, Color::Orange }},
    // FL
    { Edge::FL, { {Face::Front, 1, 0}, {Face::Left, 1, 2} },
      { Color::Green, Color::Orange }},
    // DF
    { Edge::DF, { {Face::Down, 0, 1}, {Face::Front, 2, 1} },
      { Color::Yellow, Color::Green }},
    // DR
    { Edge::DR, { {Face::Down, 1, 2}, {Face::Right, 2, 1} },
      { Color::Yellow, Color::Red }},
    // DB
    { Edge::DB, { {Face::Down, 2, 1}, {Face::Back, 2, 1} },
      { Color::Yellow, Color::Blue }},
    // DL
    { Edge::DL, { {Face::Down, 1, 0}, {Face::Left, 2, 1} },
      { Color::Yellow, Color::Orange }}
};

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

void Cube::print_face_row(Color f[6][3][3], Face face, int row) const
{
	for (int c = 0; c < 3; c++)
	{
		printf("%c ", color_to_char(f[face_to_index(face)][row][c]));
	}
	printf(" ");
}

void Cube::to_faces(Color out_faces[6][3][3]) const {
    // 1. Fill each face with its solid colour (centre stickers)
    for (int f = 0; f < 6; ++f)
        for (int r = 0; r < 3; ++r)
            for (int c = 0; c < 3; ++c)
                out_faces[f][r][c] = FACE_COLORS[f];

    // 2. Place corner stickers according to permutation & orientation
    for (int i = 0; i < 8; ++i) {
        int piece = state.cp[i];
        int ori   = state.co[i];
        const CornerInfo &ci = CORNERS[piece];
        for (int k = 0; k < 3; ++k) {
            const Facelet &fl = ci.facelets[(k + ori) % 3];
            out_faces[face_to_index(fl.face)][fl.row][fl.col] = ci.solved[k];
        }
    }

    // 3. Place edge stickers according to permutation & orientation
    for (int i = 0; i < 12; ++i) {
        int piece = state.ep[i];
        int ori   = state.eo[i];
        const EdgeInfo &ei = EDGES[piece];
        for (int k = 0; k < 2; ++k) {
            const Facelet &fl = ei.facelets[(k + ori) % 2];
            out_faces[face_to_index(fl.face)][fl.row][fl.col] = ei.solved[k];
        }
    }
}

void Cube::print() const
{
	Color f[6][3][3];
	to_faces(f);

	for (int r = 0; r < 3; r++) {
		printf("       ");
		print_face_row(f, Face::Up, r);
		printf("\n");
	}
	printf("\n");

	for (int r = 0; r < 3; r++) {
		print_face_row(f, Face::Left, r);
		print_face_row(f, Face::Front, r);
		print_face_row(f, Face::Right, r);
		print_face_row(f, Face::Back, r);
		printf("\n");
	}
	printf("\n");

	for (int r = 0; r < 3; r++) {
		printf("       ");
		print_face_row(f, Face::Down, r);
		printf("\n");
	}
	printf("\n");
	printf("\n");
}


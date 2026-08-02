#pragma once

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

const Face FACES[6] = {
	Face::Up,
	Face::Down,
	Face::Front,
	Face::Back,
	Face::Left,
	Face::Right,
};

enum class Corner {
	UFR, URB, UBL, ULF,
	DFR, DRB, DBL, DLF,
};

enum class Edge {
	UF, UR, UB, UL,
	FR, BR, BL, FL,
	DF, DR, DB, DL,
};

static constexpr u8 COR_PIECE_MASK = 0b00000111;
static constexpr u8 COR_ORI_MASK   = 0b00011000;
static constexpr u8 COR_ORI_SHIFT  = 3;

class CornerState {
private:
	u8 data;

public:
	constexpr CornerState() : data(0) {}

	constexpr CornerState(Corner piece, u8 ori)
			: data(((u8)piece & COR_PIECE_MASK) | ((ori & 0b11) << COR_ORI_SHIFT)) {}

	static constexpr CornerState solved(Corner piece)
	{
		return CornerState(piece, 0);
	}

	constexpr Corner piece() const
	{
		return (Corner)(data & COR_PIECE_MASK);
	}

	constexpr u8 orientation() const
	{
		return (data & COR_ORI_MASK) >> COR_ORI_SHIFT;
	}

	constexpr void set(Corner piece, u8 ori)
	{
		data = ((u8)piece & COR_PIECE_MASK) | ((ori & 0b11) << COR_ORI_SHIFT);
	}

	constexpr void set_piece(Corner piece)
	{
		data = (data & ~COR_PIECE_MASK) | ((u8)piece & COR_PIECE_MASK);
	}

	constexpr void set_orientation(u8 ori)
	{
		data = (data & ~COR_ORI_MASK) | ((ori & 0b11) << COR_ORI_SHIFT);
	}

	constexpr u8 raw() const
	{
		return data;
	}

	constexpr bool operator==(const CornerState&) const = default;
	constexpr bool operator!=(const CornerState&) const = default;
};

static constexpr u8 ED_PIECE_MASK = 0b00001111;
static constexpr u8 ED_ORI_MASK   = 0b00010000;
static constexpr u8 ED_ORI_SHIFT  = 4;

class EdgeState {
private:
	u8 data;

public:
	constexpr EdgeState() : data(0) {}

	constexpr EdgeState(Edge piece, u8 ori)
			: data(((u8)piece & ED_PIECE_MASK) | ((ori & 1) << ED_ORI_SHIFT)) {}

	static constexpr EdgeState solved(Edge piece)
	{
		return EdgeState(piece, 0);
	}

	constexpr Edge piece() const
	{
		return (Edge)(data & ED_PIECE_MASK);
	}

	constexpr u8 orientation() const
	{
		return (data & ED_ORI_MASK) >> ED_ORI_SHIFT;
	}

	constexpr void set(Edge piece, u8 ori)
	{
		data = ((u8)piece & ED_PIECE_MASK) | ((ori & 1) << ED_ORI_SHIFT);
	}

	constexpr void set_piece(Edge piece)
	{
		data = (data & ~ED_PIECE_MASK) | ((u8)piece & ED_PIECE_MASK);
	}

	constexpr void set_orientation(u8 ori)
	{
		data = (data & ~ED_ORI_MASK) | ((ori & 1) << ED_ORI_SHIFT);
	}

	constexpr u8 raw() const
	{
		return data;
	}

	constexpr bool operator==(const EdgeState&) const = default;
	constexpr bool operator!=(const EdgeState&) const = default;
};

struct CubeState {
	CornerState corners[8];
	EdgeState edges[12];
};

struct Facelet {
    Face face;
    u8 row;
    u8 col;
};

struct CornerInfo {
	Corner piece;
	Facelet facelets[3];
	Color solved[3];

	bool same_corner(const Color c[3]) const
	{
		return (c[0] == solved[0] || c[0] == solved[1] || c[0] == solved[2]) &&
					 (c[1] == solved[0] || c[1] == solved[1] || c[1] == solved[2]) &&
					 (c[2] == solved[0] || c[2] == solved[1] || c[2] == solved[2]);
	}
};

struct EdgeInfo {
	Edge piece;
	Facelet facelets[2];
	Color solved[2];

	bool hasUD;

	bool same_edge(const Color c[2]) const
	{
		return (c[0] == solved[0] || c[0] == solved[1]) &&
					 (c[1] == solved[0] || c[1] == solved[1]);
	}
};

constexpr CornerInfo CORNERS[8] = {
	// UFR
	{
		Corner::UFR,
		{
			{Face::Up,    2, 2},
			{Face::Front, 0, 2},
			{Face::Right, 0, 0},
		},
		{
			Color::White,
			Color::Green,
			Color::Red
		}
	},

	// URB
	{
		Corner::URB,
		{
			{Face::Up,    0, 2},
			{Face::Right, 0, 2},
			{Face::Back,  0, 0},
		},
		{
			Color::White,
			Color::Red,
			Color::Blue
		}
	},

	// UBL
	{
		Corner::UBL,
		{
				{Face::Up,    0, 0},
				{Face::Back,  0, 2},
				{Face::Left,  0, 0},
		},
		{
			Color::White,
			Color::Blue,
			Color::Orange
		}
	},

	// ULF
	{
		Corner::ULF,
		{
			{Face::Up,    2, 0},
			{Face::Left,  0, 2},
			{Face::Front, 0, 0},
		},
		{
			Color::White,
			Color::Orange,
			Color::Green
		}
	},

	// DFR
	{
		Corner::DFR,
		{
			{Face::Down,  0, 2},
			{Face::Front, 2, 2},
			{Face::Right, 2, 0},
		},
		{
			Color::Yellow,
			Color::Green,
			Color::Red
		}
	},

	// DRB
	{
		Corner::DRB,
		{
			{Face::Down,  2, 2},
			{Face::Right, 2, 2},
			{Face::Back,  2, 0},
		},
		{
			Color::Yellow,
			Color::Red,
			Color::Blue
		}
	},

	// DBL
	{
		Corner::DBL,
		{
			{Face::Down,  2, 0},
			{Face::Back,  2, 2},
			{Face::Left,  2, 0},
		},
		{
			Color::Yellow,
			Color::Blue,
			Color::Orange
		}
	},

	// DLF
	{
		Corner::DLF,
		{
			{Face::Down,  0, 0},
			{Face::Left,  2, 2},
			{Face::Front, 2, 0},
		},
		{
			Color::Yellow,
			Color::Orange,
			Color::Green
		}
	},
};

constexpr EdgeInfo EDGES[12] = {
	// UF
	{
		Edge::UF,
		{
			{Face::Up,    2, 1},
			{Face::Front, 0, 1},
		},
		{
			Color::White,
			Color::Green
		},
		true,
	},

	// UR
	{
		Edge::UR,
		{
			{Face::Up,    1, 2},
			{Face::Right, 0, 1},
		},
		{
			Color::White,
			Color::Red
		},
		true,
	},

	// UB
	{
		Edge::UB,
		{
			{Face::Up,    0, 1},
			{Face::Back,  0, 1},
		},
		{
			Color::White,
			Color::Blue
		},
		true,
	},

	// UL
	{
		Edge::UL,
		{
			{Face::Up,    1, 0},
			{Face::Left,  0, 1},
		},
		{
			Color::White,
			Color::Orange
		},
		true,
	},

	// FR
	{
		Edge::FR,
		{
			{Face::Front, 1, 2},
			{Face::Right, 1, 0},
		},
		{
			Color::Green,
			Color::Red
		},
		false,
	},

	// BR
	{
		Edge::BR,
		{
			{Face::Back,  1, 0},
			{Face::Right, 1, 2},
		},
		{
			Color::Blue,
			Color::Red
		},
		false,
	},

	// BL
	{
		Edge::BL,
		{
			{Face::Back,  1, 2},
			{Face::Left,  1, 0},
		},
		{
			Color::Blue,
			Color::Orange
		},
		false,
	},

	// FL
	{
		Edge::FL,
		{
			{Face::Front, 1, 0},
			{Face::Left,  1, 2},
		},
		{
			Color::Green,
			Color::Orange
		},
		false,
	},

	// DF
	{
		Edge::DF,
		{
			{Face::Down,  0, 1},
			{Face::Front, 2, 1},
		},
		{
			Color::Yellow,
			Color::Green
		},
		true,
	},

	// DR
	{
		Edge::DR,
		{
			{Face::Down,  1, 2},
			{Face::Right, 2, 1},
		},
		{
			Color::Yellow,
			Color::Red
		},
		true,
	},

	// DB
	{
		Edge::DB,
		{
			{Face::Down,  2, 1},
			{Face::Back,  2, 1},
		},
		{
			Color::Yellow,
			Color::Blue
		},
		true,
	},

	// DL
	{
		Edge::DL,
		{
			{Face::Down,  1, 0},
			{Face::Left,  2, 1},
		},
		{
			Color::Yellow,
			Color::Orange
		},
		true,
	},
};

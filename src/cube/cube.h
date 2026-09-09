#pragma once

#include "../utils/types.h"

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

enum class Corner {
	UFR, URB, UBL, ULF,
	DFR, DRB, DBL, DLF,
};

enum class Edge {
	UF, UR, UB, UL,
	FR, BR, BL, FL,
	DF, DR, DB, DL,
};

// 8 corners: URF=0, UFL=1, ULB=2, UBR=3, DFR=4, DLF=5, DBL=6, DRB=7
// 12 edges:  UR=0,  UF=1,  UL=2,  UB=3,  DR=4,  DF=5,  DL=6,  DB=7,  FR=8, FL=9, BL=10, BR=11

struct CubeState {
	u8 cp[8];
	u8 co[8];

	u8 ep[12];
	u8 eo[12];

	bool operator==(const CubeState &other) const;
	bool operator!=(const CubeState &other) const;
};

class Cube {
public:
	Cube();

	void reset();
	bool is_solved() const;
	void print() const;

	void apply_move(Move m);
	void apply_moves(const vector<Move>& moves);
	void apply_algorithm(string alg);

	static vector<Move> gen_random_scramble(int length = 25);

	void scramble(int length = 25);
	void scramble(string s);

	const CubeState &get_state() const { return state; }
	void set_state(const CubeState &s) { state = s; }

    // Phase 1 Coordinates
    u16 get_twist() const;      // 0..3^7
    u16 get_flip() const;       // 0..2^11
    u16 get_uds_slice() const;  // 0..C(12,4)

    // Phase 2 Coordinates
    u16 get_corner_perm() const;   // 0..8!
    u16 get_ud_edges_perm() const; // 0..8!
    u8  get_slice_perm() const;    // 0..4!
    
	// Setters
    void set_twist(u16 twist);
    void set_flip(u16 flip);
    void set_uds_slice(u16 slice);
    void set_corner_perm(u16 cp);
    void set_ud_edges_perm(u16 ep);
    void set_slice_perm(u8 sep);
    
	// Permutation multiplication
    static void multiply(const CubeState& a, const CubeState& b, CubeState& result);
    
    static CubeState MOVE_TRANSFORMS[18];
    static void init_move_transforms();

	bool operator==(const Cube &other) const;
	bool operator!=(const Cube &other) const;

private:
	CubeState state;

	void print_face_row(Color f[6][3][3], Face face, int row) const;
	void to_faces(Color out_faces[6][3][3]) const;
};

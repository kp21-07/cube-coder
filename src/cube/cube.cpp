#include "cube.h"
/*
╔═══════════════════════════════════════════════════════════════╗
║                             UTILS                             ║
╚═══════════════════════════════════════════════════════════════╝
*/

Cube::Cube() { reset(); }

void Cube::reset()
{
	for (int i = 0; i < 8; i++) {
		state.cp[i] = i;
		state.co[i] = 0;
	}
	for (int i = 0; i < 12; i++) {
		state.ep[i] = i;
		state.eo[i] = 0;
	}
}

bool Cube::is_solved() const 
{
    for (int i = 0; i < 8; i++) {
        if (state.cp[i] != i || state.co[i] != 0) return false;
    }
    for (int i = 0; i < 12; i++) {
        if (state.ep[i] != i || state.eo[i] != 0) return false;
    }
    return true;
}

/*
╔═══════════════════════════════════════════════════════════════╗
║                        MOVE FUNCTIONS                         ║
╚═══════════════════════════════════════════════════════════════╝
*/

void Cube::multiply(const CubeState& a, const CubeState& b, CubeState& result)
{
	for (int i = 0; i < 8; i++) {
		result.cp[i] = a.cp[b.cp[i]];
		result.co[i] = (a.co[b.cp[i]] + b.co[i]) % 3;
	}

	for (int i = 0; i < 12; i++) {
		result.ep[i] = a.ep[b.ep[i]];
		result.eo[i] = (a.eo[b.ep[i]] + b.eo[i]) % 2;
	}
}

static const CubeState BASIC_MOVES[6] =
{
    // U Move (Face 0)
    {
        { 3, 0, 1, 2, 4, 5, 6, 7 }, // cp: UBR->URF, URF->UFL, UFL->ULB, ULB->UBR
        { 0, 0, 0, 0, 0, 0, 0, 0 }, // co: all 0
        { 3, 0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11 }, // ep: UB->UR, UR->UF, UF->UL, UL->UB
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    // eo: all 0
    },
    // R Move (Face 1)
    {
        { 4, 1, 2, 0, 7, 5, 6, 3 }, // cp
        { 2, 0, 0, 1, 1, 0, 0, 2 }, // co: U/D axis twists
        { 8, 1, 2, 3, 11, 5, 6, 7, 4, 9, 10, 0 }, // ep
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    // eo: all 0
    },
    // F Move (Face 2)
    {
        { 1, 5, 2, 3, 0, 4, 6, 7 }, // cp
        { 1, 2, 0, 0, 2, 1, 0, 0 }, // co
        { 0, 9, 2, 3, 4, 8, 6, 7, 1, 5, 10, 11 }, // ep
        { 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 }    // eo: 4 edges flipped
    },
    // D Move (Face 3)
    {
        { 0, 1, 2, 3, 5, 6, 7, 4 }, // cp
        { 0, 0, 0, 0, 0, 0, 0, 0 }, // co
        { 0, 1, 2, 3, 5, 6, 7, 4, 8, 9, 10, 11 }, // ep
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    // eo
    },
    // L Move (Face 4)
    {
        { 0, 2, 6, 3, 4, 1, 5, 7 }, // cp
        { 0, 1, 2, 0, 0, 2, 1, 0 }, // co
        { 0, 1, 10, 3, 4, 5, 9, 7, 8, 2, 6, 11 }, // ep
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }    // eo: all 0
    },
    // B Move (Face 5)
    {
        { 0, 1, 3, 7, 4, 5, 2, 6 }, // cp
        { 0, 0, 1, 2, 0, 0, 2, 1 }, // co
        { 0, 1, 2, 11, 4, 5, 6, 10, 8, 9, 3, 7 }, // ep
        { 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }    // eo: 4 edges flipped
    }
};

CubeState Cube::MOVE_TRANSFORMS[18];

void Cube::init_move_transforms()
{
    static bool initialized = false;
    if (initialized) return;
    // For each face: Move = basic, Move2 = Move * Move, MovePrime = Move2 * Move
    for (int f = 0; f < 6; f++) {
        MOVE_TRANSFORMS[f * 3 + 0] = BASIC_MOVES[f];
        multiply(BASIC_MOVES[f], BASIC_MOVES[f], MOVE_TRANSFORMS[f * 3 + 2]); // Move2
        multiply(MOVE_TRANSFORMS[f * 3 + 2], BASIC_MOVES[f], MOVE_TRANSFORMS[f * 3 + 1]); // MovePrime
    }
    initialized = true;
}

void Cube::apply_move(Move m)
{
    CubeState next;
    multiply(state, MOVE_TRANSFORMS[(int)m], next);
    state = next;
}

void Cube::apply_moves(const vector<Move>& moves)
{
	for (Move move : moves) apply_move(move);
}

void Cube::apply_algorithm(string alg)
{
	apply_moves(parse_alg(alg));
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

void Cube::scramble(int length)
{
	apply_moves(gen_random_scramble(length));
}

void Cube::scramble(string alg)
{
	apply_algorithm(alg);
}


/*
╔═══════════════════════════════════════════════════════════════╗
║                      GETTERS AND SETTERS                      ║
╚═══════════════════════════════════════════════════════════════╝
*/

// --- Phase 1 ---
u16 Cube::get_twist() const
{
    u16 twist = 0;
    for (int i = 0; i < 7; i++) twist = twist * 3 + state.co[i];
    return twist;
}

u16 Cube::get_flip() const
{
    u16 flip = 0;
    for (int i = 0; i < 11; i++) flip = flip * 2 + state.eo[i];
    return flip;
}

u16 Cube::get_uds_slice() const
{
    u16 slice = 0;
    int k = 3;
	static const int C[12][5] = {
		{1},{1,1},{1,2,1},{1,3,3,1},{1,4,6,4,1},
		{1,5,10,10,5},{1,6,15,20,15},{1,7,21,35,35},
		{1,8,28,56,70},{1,9,36,84,126},{1,10,45,120,210},
		{1,11,55,165,330}
	};
    for (int i = 11; i >= 0 && k >= 0; i--) {
        if (state.ep[i] >= 8) { // Is an E-slice edge
            k--;
        } else {
            // Add C(i, k)
            if (k >= 0) slice += C[i][k];
        }
    }
    return slice;
}

void Cube::set_twist(u16 twist)
{
    reset();
	int sum = 0;
	
	for (int i = 6; i >= 0; i--) {
		state.co[i] = twist%3;
		twist /= 3;
		sum += state.co[i];
	}
	
	state.co[7] = (3 - (sum%3)) % 3;
}

void Cube::set_flip(u16 twist)
{
    reset();
	int sum = 0;
	
	for (int i = 10; i >= 0; i--) {
		state.eo[i] = twist%2;
		twist /= 2;
		sum += state.eo[i];
	}
	
	state.eo[11] = (2 - (sum%2)) % 2;
}

void Cube::set_uds_slice(u16 slice)
{
    reset();
    int k = 3;
	static const int C[12][5] = {
		{1},{1,1},{1,2,1},{1,3,3,1},{1,4,6,4,1},
		{1,5,10,10,5},{1,6,15,20,15},{1,7,21,35,35},
		{1,8,28,56,70},{1,9,36,84,126},{1,10,45,120,210},
		{1,11,55,165,330}
	};

	int e_edge = 8;
	int nor_edge = 0;
    for (int i = 11; i >= 0; i--) {
		if (k >= 0 && slice < C[i][k]) {
			state.ep[i] = e_edge++;
			k--;
		}
		else {
			if (k >= 0) slice -= C[i][k];
			state.ep[i] = nor_edge++;
		}
	}
}

// --- Phase 2 ---
u16 Cube::get_corner_perm() const
{
    // Lehmer code for 8 corners
    u16 cp_idx = 0;
    static const int FACT[8] = { 5040, 720, 120, 24, 6, 2, 1, 1 };
    
	for (int i = 0; i < 7; i++) {
        int count = 0;
        for (int j = i + 1; j < 8; j++)
            if (state.cp[j] < state.cp[i]) count++;
    
		cp_idx += count * FACT[i];
    }
    
	return cp_idx;
}

u16 Cube::get_ud_edges_perm() const
{
    // Lehmer code for the 8 U/D edges (0..7)
    u16 ep_idx = 0;
    static const int FACT[8] = { 5040, 720, 120, 24, 6, 2, 1, 1 };
    
	for (int i = 0; i < 7; i++) {
        int count = 0;
        for (int j = i + 1; j < 8; j++)
            if (state.ep[j] < state.ep[i]) count++;
    
		ep_idx += count * FACT[i];
    }
    
	return ep_idx;
}

u8 Cube::get_slice_perm() const
{
    // Lehmer code for the 4 E-slice edges (8..11)
    u8 sep = 0;
    static const int FACT[4] = { 6, 2, 1, 1 };

    for (int i = 8; i < 11; i++) {
        int count = 0;
        for (int j = i + 1; j < 12; j++)
            if (state.ep[j] < state.ep[i]) count++;

        sep += count * FACT[i - 8];
    }

    return sep;
}

void Cube::set_corner_perm(u16 cp)
{
    reset();
    static const int FACT[8] = { 5040, 720, 120, 24, 6, 2, 1, 1 };
	int available[8] = {0,1,2,3,4,5,6,7};

	for (int i = 0; i < 7; i++) {
		int count = cp / FACT[i];
		cp %= FACT[i];
		state.cp[i] = available[count];

		for (int j = count; j < 7-i; j++) available[j] = available[j+1];
	}

	state.cp[7] = available[0];
}

void Cube::set_ud_edges_perm(u16 ep)
{
    reset();
    static const int FACT[8] = { 5040, 720, 120, 24, 6, 2, 1, 1 };
	int available[8] = {0,1,2,3,4,5,6,7};

	for (int i = 0; i < 7; i++) {
		int count = ep / FACT[i];
		ep %= FACT[i];
		state.ep[i] = available[count];

		for (int j = count; j < 7-i; j++) available[j] = available[j+1];
	}

	state.ep[7] = available[0];
}

void Cube::set_slice_perm(u8 ep)
{
    reset();
    static const int FACT[4] = { 6, 2, 1, 1 };
	int available[4] = {8,9,10,11};

	for (int i = 8; i < 11; i++) {
		int count = ep / FACT[i-8];
		ep %= FACT[i-8];
		state.ep[i] = available[count];

		for (int j = count; j < 11-i; j++) available[j] = available[j+1];
	}

	state.ep[11] = available[0];
}

/*
╔═══════════════════════════════════════════════════════════════╗
║                     OPERATOR OVERLOADING                      ║
╚═══════════════════════════════════════════════════════════════╝
*/

bool Cube::operator==(const Cube& other) const
{
    for (int i = 0; i < 8; i++) {
        if (state.co[i] != other.state.co[i]) return false;
		if (state.cp[i] != other.state.cp[i]) return false;
	}

    for (int i = 0; i < 12; i++) {
        if (state.eo[i] != other.state.eo[i]) return false;
		if (state.ep[i] != other.state.ep[i]) return false;
	}

    return true;
}

bool Cube::operator!=(const Cube& other) const
{
    return !(*this == other);
}

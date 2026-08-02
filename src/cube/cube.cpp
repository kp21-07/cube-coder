#include "cube.h"
#include "cubie.h"
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
	for (int i = 0; i < 8; i++) {
		state.corners[i].set((Corner) i, 0);
	}
	for (int i = 0; i < 12; i++) {
		state.edges[i].set((Edge) i, 0);
	}
}

bool Cube::is_solved() const
{
	for (int i = 0; i < 8; i++) {
		if (state.corners[i].piece() != (Corner)i || state.corners[i].orientation() != 0) return false;
	}
	for (int i = 0; i < 12; i++) {
		if (state.edges[i].piece() != (Edge)i || state.edges[i].orientation() != 0) return false;
	}
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

void Cube::print_face_row(Color f[6][3][3], Face face, int row) const
{
	for (int c = 0; c < 3; c++)
	{
		printf("%c ", color_to_char(f[face_to_index(face)][row][c]));
	}
		printf(" ");
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

void Cube::U(int t)
{
	while (t--) {
		CubeState t_s = state;

		state.corners[0] = t_s.corners[1];
		state.corners[1] = t_s.corners[2];
		state.corners[2] = t_s.corners[3];
		state.corners[3] = t_s.corners[0];

		state.edges[0] = t_s.edges[1];
		state.edges[1] = t_s.edges[2];
		state.edges[2] = t_s.edges[3];
		state.edges[3] = t_s.edges[0];
	}
}

void Cube::D(int t)
{
	while (t--) {
		CubeState t_s = state;

		state.corners[4] = t_s.corners[7];
		state.corners[5] = t_s.corners[4];
		state.corners[6] = t_s.corners[5];
		state.corners[7] = t_s.corners[6];

		state.edges[8] = t_s.edges[11];
		state.edges[9] = t_s.edges[8];
		state.edges[10] = t_s.edges[9];
		state.edges[11] = t_s.edges[10];
	}
}

void Cube::R(int t)
{
	while (t--) {
		CubeState t_s = state;

		state.corners[0] = t_s.corners[4];
		state.corners[0].set_orientation((state.corners[0].orientation() + 1) % 3);

		state.corners[1] = t_s.corners[0];
		state.corners[1].set_orientation((state.corners[1].orientation() + 2) % 3);

		state.corners[4] = t_s.corners[5];
		state.corners[4].set_orientation((state.corners[4].orientation() + 1) % 3);

		state.corners[5] = t_s.corners[1];
		state.corners[5].set_orientation((state.corners[5].orientation() + 2) % 3);


		state.edges[1] = t_s.edges[4];
		state.edges[1].set_orientation((state.edges[1].orientation() + 1) % 2);

		state.edges[4] = t_s.edges[9];
		state.edges[4].set_orientation((state.edges[4].orientation() + 1) % 2);

		state.edges[5] = t_s.edges[1];
		state.edges[5].set_orientation((state.edges[5].orientation() + 1) % 2);

		state.edges[9] = t_s.edges[5];
		state.edges[9].set_orientation((state.edges[9].orientation() + 1) % 2);
	}
}

void Cube::L(int t)
{
	while (t--) {
		CubeState t_s = state;

		state.corners[2] = t_s.corners[6];
		state.corners[2].set_orientation((state.corners[2].orientation() + 1) % 3);

		state.corners[3] = t_s.corners[2];
		state.corners[3].set_orientation((state.corners[3].orientation() + 2) % 3);

		state.corners[6] = t_s.corners[7];
		state.corners[6].set_orientation((state.corners[6].orientation() + 1) % 3);

		state.corners[7] = t_s.corners[3];
		state.corners[7].set_orientation((state.corners[7].orientation() + 2) % 3);


		state.edges[3] = t_s.edges[6];
		state.edges[3].set_orientation((state.edges[3].orientation() + 1) % 2);

		state.edges[6] = t_s.edges[11];
		state.edges[6].set_orientation((state.edges[6].orientation() + 1) % 2);

		state.edges[7] = t_s.edges[3];
		state.edges[7].set_orientation((state.edges[7].orientation() + 1) % 2);

		state.edges[11] = t_s.edges[7];
		state.edges[11].set_orientation((state.edges[11].orientation() + 1) % 2);
	}
}

void Cube::F(int t)
{
	while (t--) {
		CubeState t_s = state;

		state.corners[0] = t_s.corners[3];
		state.corners[0].set_orientation((state.corners[0].orientation() + 2) % 3);
		
		state.corners[3] = t_s.corners[7];
		state.corners[3].set_orientation((state.corners[3].orientation() + 1) % 3);
		
		state.corners[4] = t_s.corners[0];
		state.corners[4].set_orientation((state.corners[4].orientation() + 2) % 3);
		
		state.corners[7] = t_s.corners[4];
		state.corners[7].set_orientation((state.corners[7].orientation() + 1) % 3);
		

		state.edges[0] = t_s.edges[7];
		state.edges[0].set_orientation((state.edges[0].orientation() + 1) % 2);
		
		state.edges[4] = t_s.edges[0];
		state.edges[7] = t_s.edges[8];
		
		state.edges[8] = t_s.edges[4];
		state.edges[8].set_orientation((state.edges[8].orientation() + 1) % 2);
	}
}

void Cube::B(int t)
{
	while (t--) {
		CubeState t_s = state;
		
		state.corners[1] = t_s.corners[5];
		state.corners[1].set_orientation((state.corners[1].orientation() + 1) % 3);
		
		state.corners[2] = t_s.corners[1];
		state.corners[2].set_orientation((state.corners[2].orientation() + 2) % 3);
		
		state.corners[5] = t_s.corners[6];
		state.corners[5].set_orientation((state.corners[5].orientation() + 1) % 3);
		
		state.corners[6] = t_s.corners[2];
		state.corners[6].set_orientation((state.corners[6].orientation() + 2) % 3);
		

		state.edges[2] = t_s.edges[5];
		state.edges[2].set_orientation((state.edges[2].orientation() + 1) % 2);
		
		state.edges[5] = t_s.edges[10];
		state.edges[6] = t_s.edges[2];
		
		state.edges[10] = t_s.edges[6];
		state.edges[10].set_orientation((state.edges[10].orientation() + 1) % 2);
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

CubeState Cube::extract_state() const
{
	return state;
}

bool Cube::operator==(const Cube& other) const
{
	for (int i = 0; i < 8; i++) {
		if (state.corners[i].piece() != other.state.corners[i].piece() || state.corners[i].orientation() != other.state.corners[i].orientation()) return false;
	}
	for (int i = 0; i < 12; i++) {
		if (state.edges[i].piece() != other.state.edges[i].piece() || state.edges[i].orientation() != other.state.edges[i].orientation()) return false;
	}
	return true;
}

bool Cube::operator!=(const Cube& other) const
{
	return !(*this == other);
}

void Cube::to_faces(Color out_faces[6][3][3]) const
{
    for (int f = 0; f < 6; f++) {
        for (int r = 0; r < 3; r++) {
            for (int c = 0; c < 3; c++) {
                out_faces[f][r][c] = FACE_COLORS[f];
            }
        }
    }

    auto face_to_index = [](Face face) {
        switch (face) {
            case Face::Up    : return 0;
            case Face::Down  : return 1;
            case Face::Front : return 2;
            case Face::Back  : return 3;
            case Face::Left  : return 4;
            case Face::Right : return 5;
        }
        return 0;
    };

    for (int i = 0; i < 8; i++) {
        CornerState cs = state.corners[i];
        for (int k = 0; k < 3; k++) {
            Facelet f = CORNERS[i].facelets[(k + cs.orientation()) % 3];
            out_faces[face_to_index(f.face)][f.row][f.col] = CORNERS[(int)cs.piece()].solved[k];
        }
    }

    for (int i = 0; i < 12; i++) {
        EdgeState es = state.edges[i];
        for (int k = 0; k < 2; k++) {
            Facelet f = EDGES[i].facelets[(k + es.orientation()) % 2];
            out_faces[face_to_index(f.face)][f.row][f.col] = EDGES[(int)es.piece()].solved[k];
        }
    }
}

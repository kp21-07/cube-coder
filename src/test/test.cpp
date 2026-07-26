#include "test.h"
#include "../cube/cube.h"
#include "../cube/move.h"
#include "../solver/solve.h"
#include "../utils/types.h"
#include <stdio.h>
#include <time.h>
#include <iostream>

// -------------------------------------------------------
// Internal helpers
// -------------------------------------------------------

static int s_pass = 0;
static int s_fail = 0;

static void check(bool condition, const char* label)
{
	if (condition) {
		printf("  PASS  %s\n", label);
		s_pass++;
	} else {
		printf("  FAIL  %s\n", label);
		s_fail++;
	}
}

static void section(const char* title)
{
	printf("\n--- %s ---\n", title);
}

static const char* move_name(Move m)
{
	switch (m)
	{
		case Move::U:       return "U";
		case Move::U_Prime: return "U'";
		case Move::U2:      return "U2";
		case Move::R:       return "R";
		case Move::R_Prime: return "R'";
		case Move::R2:      return "R2";
		case Move::F:       return "F";
		case Move::F_Prime: return "F'";
		case Move::F2:      return "F2";
		case Move::D:       return "D";
		case Move::D_Prime: return "D'";
		case Move::D2:      return "D2";
		case Move::L:       return "L";
		case Move::L_Prime: return "L'";
		case Move::L2:      return "L2";
		case Move::B:       return "B";
		case Move::B_Prime: return "B'";
		case Move::B2:      return "B2";
		default:            return "???";
	}
}

// -------------------------------------------------------
// test_all_moves — visual inspection
// -------------------------------------------------------

void test_all_moves()
{
	section("All moves (visual)");

	Move moves[] = {
		Move::U, Move::U_Prime, Move::U2,
		Move::R, Move::R_Prime, Move::R2,
		Move::F, Move::F_Prime, Move::F2,
		Move::D, Move::D_Prime, Move::D2,
		Move::L, Move::L_Prime, Move::L2,
		Move::B, Move::B_Prime, Move::B2,
	};
	int count = (int)(sizeof(moves) / sizeof(moves[0]));

	for (int i = 0; i < count; i++) {
		Cube cube;
		printf("\n  [%s]\n", move_name(moves[i]));
		cube.apply_move(moves[i]);
		cube.print();
	}
}

// -------------------------------------------------------
// test_sanity_x4 — every CW move x4 = solved
// -------------------------------------------------------

void test_sanity_x4()
{
	section("x4 = solved");

	struct Case { Move m; const char* name; };
	Case cases[] = {
		{ Move::U, "U x4" }, { Move::R, "R x4" }, { Move::F, "F x4" },
		{ Move::D, "D x4" }, { Move::L, "L x4" }, { Move::B, "B x4" },
	};

	for (int i = 0; i < 6; i++) {
		Cube cube;
		cube.apply_move(cases[i].m);
		cube.apply_move(cases[i].m);
		cube.apply_move(cases[i].m);
		cube.apply_move(cases[i].m);
		check(cube.is_solved(), cases[i].name);
	}
}

// -------------------------------------------------------
// test_sanity_prime — move + prime = solved
// -------------------------------------------------------

void test_sanity_prime()
{
	section("move + prime = solved");

	struct Case { Move m; Move prime; const char* name; };
	Case cases[] = {
		{ Move::U, Move::U_Prime, "U + U'" },
		{ Move::R, Move::R_Prime, "R + R'" },
		{ Move::F, Move::F_Prime, "F + F'" },
		{ Move::D, Move::D_Prime, "D + D'" },
		{ Move::L, Move::L_Prime, "L + L'" },
		{ Move::B, Move::B_Prime, "B + B'" },
	};

	for (int i = 0; i < 6; i++) {
		Cube cube;
		cube.apply_move(cases[i].m);
		cube.apply_move(cases[i].prime);
		check(cube.is_solved(), cases[i].name);
	}
}

// -------------------------------------------------------
// test_scramble — scramble then check cube is not solved,
//                 and that it has the right number of moves
// -------------------------------------------------------

void test_scramble()
{
	section("Random scramble");

	Cube cube;
	vector<Move> scramble = cube.gen_random_scramble(20);
	cube.apply_moves(scramble);

	check(scramble.size() == 20,   "scramble has 20 moves");
	check(!cube.is_solved(),       "cube is not solved after scramble");

	// Verify no adjacent same-face or opposite-face moves
	// Face group: U=0,R=1,F=2,D=3,L=4,B=5  opposite[i] = i^3 for U/D/R/L/F/B pairs
	static const int face_of[] = { 0,0,0, 1,1,1, 2,2,2, 3,3,3, 4,4,4, 5,5,5 };
	static const int opposite[] = { 3, 4, 5, 0, 1, 2 };

	bool no_repeat = true;
	for (int i = 1; i < (int)scramble.size(); i++) {
		int prev = face_of[(int)scramble[i-1]];
		int curr = face_of[(int)scramble[i]];
		if (curr == prev || curr == opposite[prev]) {
			no_repeat = false;
			break;
		}
	}
	check(no_repeat, "no adjacent same/opposite-face moves");
}

// -------------------------------------------------------
// test_inverse — scramble + inverse = solved
// -------------------------------------------------------

void test_inverse()
{
	section("Inverse algorithm");

	// Fixed known case
	{
		Cube cube;
		cube.apply_algorithm("R U R' U'");
		cube.apply_algorithm(inverse_alg("R U R' U'"));
		check(cube.is_solved(), "inverse(\"R U R' U'\") restores cube");
	}

	// Random scramble case
	{
		Cube cube;
		vector<Move> scramble = cube.gen_random_scramble(20);
		cube.apply_moves(scramble);

		string alg    = to_string(scramble);
		string inv    = inverse_alg(alg);
		cube.apply_algorithm(inv);
		check(cube.is_solved(), "inverse of random scramble restores cube");
	}
}

// -------------------------------------------------------
// run_tests — entry point
// -------------------------------------------------------

void test_move_engine()
{
	srand((unsigned int)time(nullptr));

	s_pass = s_fail = 0;

	test_all_moves();
	test_sanity_x4();
	test_sanity_prime();
	test_scramble();
	test_inverse();

	printf("\n==============================\n");
	printf("  Results: %d passed, %d failed\n", s_pass, s_fail);
	printf("==============================\n");
}






// -------------------------------------------------------
// test_solver — IDDFS correctness
// -------------------------------------------------------

void test_iddfs(const string& alg, int depth)
{
	section("IDDFS solver");

	bool expected = depth > 0;

	Cube cube;
	cube.apply_algorithm(alg);
	printf("    scramble:  %s", alg.data());
	printf("\n");

	int exp_sol_len = parse_alg(alg).size();

	IDDFS_Solver solver;
	bool found = solver.solve(cube, abs(depth));
	if (expected) {
		printf("    solution           : %s\n", to_string(solver.res.solution).data());

		printf("    states visited     : %lu\n", solver.res.states_visited);
		printf("    nodes explored     : %lu\n", solver.res.nodes_expanded);
		printf("    children generated : %lu\n", solver.res.children_generated);
		printf("    avg. branch factor : %.2f\n", (double) solver.res.children_generated / solver.res.nodes_expanded);
		printf("    solution depth     : %d\n",   solver.res.depth);

		printf("    elapsed time       : %.3f ms\n",    solver.res.elapsed_ms);
		printf("    states / sec       : %.0f\n",       solver.res.states_per_sec);
		printf("    throughput         : %.4f Mnodes/s\n", solver.res.mnodes_per_sec);

		check(found, "scramble solved");
		check((int) solver.res.solution.size() == exp_sol_len, "solution length is correct");

		cube.apply_algorithm(alg);
		cube.apply_moves(solver.res.solution);
		check(cube.is_solved(), "cube is solved after applying solution");
	}
	else {
		check(!found, "scramble failed as expected");
	}

	printf("\n===========================================\n\n");
}

// -------------------------------------------------------
// test_solver — IDDFS correctness
// -------------------------------------------------------

void test_ida(const string& alg, Heuristic& h)
{
	section("IDA solver");

	printf("\nUsing Heuristic : %s\n\n", h.name().data());

	Cube cube;
	cube.apply_algorithm(alg);
	printf("    scramble:  %s", alg.data());
	printf("\n");

	int exp_sol_len = parse_alg(alg).size();

	IDA_Solver solver(h);
	bool found = solver.solve(cube);
	printf("    solution           : %s\n", to_string(solver.res.solution).data());

	printf("    states visited     : %lu\n", solver.res.states_visited);
	printf("    nodes explored     : %lu\n", solver.res.nodes_expanded);
	printf("    children generated : %lu\n", solver.res.children_generated);
	printf("    avg. branch factor : %.2f\n", (double) solver.res.children_generated / solver.res.nodes_expanded);
	printf("    solution depth     : %d\n",   solver.res.depth);

	printf("    elapsed time       : %.3f ms\n",    solver.res.elapsed_ms);
	printf("    states / sec       : %.0f\n",       solver.res.states_per_sec);
	printf("    throughput         : %.4f Mnodes/s\n", solver.res.mnodes_per_sec);

	check(found, "scramble solved");
	check((int) solver.res.solution.size() == exp_sol_len, "solution length is correct");

	cube.apply_algorithm(alg);
	cube.apply_moves(solver.res.solution);
	check(cube.is_solved(), "cube is solved after applying solution");

	printf("\n===========================================\n\n");
}
void test_solver() {
	Cube c;
	
	while (true) {
		printf("\nSelect Heuristic:\n");
		printf("  0 = IDDFS\n");
		printf("  1 = Zero\n");
		printf("  2 = Orientation\n");
		printf("  3 = Misplaced Pieces\n");
		printf("  4 = Combined\n");
		printf("  5 = Combined PDB\n");
		printf(" -1 = Quit\n");
		printf("Choice: ");
		
		int n;
		if (!(std::cin >> n) || n == -1) break;

		while (true) {
			printf("Enter scramble length (-1 to go back): ");
			int len;
			if (!(std::cin >> len) || len == -1) break;

			string scramble = to_string(c.gen_random_scramble(len));

			switch (n)
			{
				case 0:
					test_iddfs(scramble, len);
					break;

				case 1: {
					ZeroHeuristic h1;
					test_ida(scramble, h1);
					break;
				}

				case 2: {
					OrientationHeuristic h1;
					test_ida(scramble, h1);
					break;
				}

				case 3: {
					MisplacedPieces h1;
					test_ida(scramble, h1);
					break;
				}

				case 4: {
					CombinedHeuristic h1;
					test_ida(scramble, h1);
					break;
				}

				case 5: {
					CombinedPDBHeuristic h1;
					test_ida(scramble, h1);
					break;
				}

				default:
					printf("Invalid choice.\n");
					break;
			}
		}
	}

	printf("\n==============================\n");
	printf("  Overall Results: %d passed, %d failed\n", s_pass, s_fail);
	printf("==============================\n");
}

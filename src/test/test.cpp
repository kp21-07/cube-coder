#include "../cube/cube.h"
#include "../solver/kociemba.h"
#include "../solver/solve.h"
#include "../utils/timer.h"

void test_solver()
{
	Cube::init_move_transforms();
	prune_table_init();

	while (true) {
		int SCRAMBLE_LEN;
		printf("Enter scramble length (-1 to exit): ");
		scanf("%d", &SCRAMBLE_LEN);
		printf("\n");

		if (SCRAMBLE_LEN == -1) {
			printf("Exiting.....\n");
			break;
		}

		string scramble_str = to_string(Cube::gen_random_scramble(SCRAMBLE_LEN));

		printf("Scramble (%d moves): %s\n\n", SCRAMBLE_LEN, scramble_str.data());

		Cube scrambled_cube;
		scrambled_cube.apply_algorithm(scramble_str);

		KociembaSolver solver;
		Timer t;
		t.start();
		vector<Move> solution = solver.solve(scrambled_cube);
		t.stop();

		printf("Time taken: %.3f ms\n\n", t.elapsed_ms());

		string sol_str = to_string(solution);
		printf("Solution (%zu moves): %s\n", solution.size(), sol_str.data());

		Cube verify_cube;
		verify_cube.apply_algorithm(scramble_str);
		verify_cube.apply_moves(solution);

		if (verify_cube.is_solved()) {
			printf("\n[SUCCESS] Cube restored to solved state!\n");
		} else {
			printf("\n[FAIL] Cube remains unsolved!\n");
		}

		printf("\n-------------------------------------------------------------------------------------------------------------------------------\n\n");
	}
}

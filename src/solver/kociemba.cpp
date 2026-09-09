#include "kociemba.h"

/*
╔═══════════════════════════════════════════════════════════════╗
║                          MOVE TABLES                          ║
╚═══════════════════════════════════════════════════════════════╝
*/

// Phase 1 move tables
u16 twist_move [N_TWIST][N_MOVES];
u16 flip_move  [N_FLIP] [N_MOVES];
u16 slice_move [N_SLICE][N_MOVES];

// Phase 2 move tables
u16 corner_perm_move[N_CORNER_PERM][N_MOVES];
u16 ud_edges_move   [N_UD_EDGES]   [N_MOVES];
u8  slice_perm_move [N_SLICE_PERM] [N_MOVES];

void coord_table_init()
{
	Cube c;

	for (int T = 0; T <	N_TWIST; T++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_twist(T);
			c.apply_move((Move) M);
			twist_move[T][M] = c.get_twist();
		}
	}

	for (int F = 0; F <	N_FLIP; F++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_flip(F);
			c.apply_move((Move) M);
			flip_move[F][M] = c.get_flip();
		}
	}

	for (int S = 0; S <	N_SLICE; S++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_uds_slice(S);
			c.apply_move((Move) M);
			slice_move[S][M] = c.get_uds_slice();
		}
	}

	for (int C = 0; C <	N_CORNER_PERM; C++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_corner_perm(C);
			c.apply_move((Move) M);
			corner_perm_move[C][M] = c.get_corner_perm();
		}
	}

	for (int E = 0; E <	N_UD_EDGES; E++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_ud_edges_perm(E);
			c.apply_move((Move) M);
			ud_edges_move[E][M] = c.get_ud_edges_perm();
		}
	}

	for (int S = 0; S <	N_SLICE_PERM; S++) {
		for (int M = 0; M < N_MOVES; M++) {
			c.set_slice_perm(S);
			c.apply_move((Move) M);
			slice_perm_move[S][M] = c.get_slice_perm();
		}
	}
}

/*
╔═══════════════════════════════════════════════════════════════╗
║                        PRUNING TABLES                         ║
╚═══════════════════════════════════════════════════════════════╝
*/

u8 slice_twist_prune [(N_SLICE * N_TWIST)/2 + 1];
u8 slice_flip_prune  [(N_SLICE * N_FLIP)/2 + 1];              // Phase 1

u8 corner_slice_prune   [(N_CORNER_PERM * N_SLICE_PERM)/2 + 1]; 
u8 ud_edges_slice_prune [(N_UD_EDGES * N_SLICE_PERM) / 2 + 1]; // Phase 2

void prune_table_init()
{
    Cube::init_move_transforms();
	coord_table_init();

    // 1. Slice-Twist Pruning Table (Phase 1)
    memset(slice_twist_prune, 0xFF, sizeof(slice_twist_prune));
    
    queue<int> q1;
    set_prune(slice_twist_prune, 0, 0);
    q1.push(0);

    while (!q1.empty()) {
        int idx = q1.front();
        q1.pop();

        int d = get_prune(slice_twist_prune, idx);
        int S = idx / N_TWIST;
        int T = idx % N_TWIST;

        for (int M = 0; M < N_MOVES; M++) {
            int ns = slice_move[S][M];
            int nt = twist_move[T][M];
            int nidx = ns * N_TWIST + nt;

            if (get_prune(slice_twist_prune, nidx) == 15) {
                set_prune(slice_twist_prune, nidx, d + 1);
                q1.push(nidx);
            }
        }
    }

	int visited = 0;
	int total = N_SLICE * N_TWIST; // 495 * 2187 = 1,082,565

	for (int i = 0; i < total; i++) {
		if (get_prune(slice_twist_prune, i) != 15) {
			visited++;
		}
	}
	printf("[DEBUG] Slice-Twist Table Filled: %d / %d\n", visited, total);

    // 2. Slice-Flip Pruning Table (Phase 1)
    memset(slice_flip_prune, 0xFF, sizeof(slice_flip_prune));
    
    queue<int> q2;
    set_prune(slice_flip_prune, 0, 0);
    q2.push(0);

    while (!q2.empty()) {
        int idx = q2.front();
        q2.pop();

        int d = get_prune(slice_flip_prune, idx);
        int S = idx / N_FLIP;
        int F = idx % N_FLIP;

        for (int M = 0; M < N_MOVES; M++) {
            int ns = slice_move[S][M];
            int nf = flip_move[F][M];
            int nidx = ns * N_FLIP + nf;

            if (get_prune(slice_flip_prune, nidx) == 15) {
                set_prune(slice_flip_prune, nidx, d + 1);
                q2.push(nidx);
            }
        }
    }

    // 3. Corner-Slice Pruning Table (Phase 2)
    memset(corner_slice_prune, 0xFF, sizeof(corner_slice_prune));
    
    queue<int> q3;
    set_prune(corner_slice_prune, 0, 0);
    q3.push(0);

    while (!q3.empty()) {
        int idx = q3.front();
        q3.pop();

        int d = get_prune(corner_slice_prune, idx);
        int C = idx / N_SLICE_PERM;
        int S = idx % N_SLICE_PERM;

        for (int i = 0; i < 10; i++) {
            int M = static_cast<int>(PHASE2_MOVES[i]);
            int nc = corner_perm_move[C][M];
            int ns = slice_perm_move[S][M];
            int nidx = nc * N_SLICE_PERM + ns;

            if (get_prune(corner_slice_prune, nidx) == 15) {
                set_prune(corner_slice_prune, nidx, d + 1);
                q3.push(nidx);
            }
        }
    }

    // 4. UD_Edges-Slice Pruning Table (Phase 2)
    memset(ud_edges_slice_prune, 0xFF, sizeof(ud_edges_slice_prune));
    
    queue<int> q4;
    set_prune(ud_edges_slice_prune, 0, 0);
    q4.push(0);

    while (!q4.empty()) {
        int idx = q4.front();
        q4.pop();

        int d = get_prune(ud_edges_slice_prune, idx);
        int E = idx / N_SLICE_PERM;
        int S = idx % N_SLICE_PERM;

        for (int i = 0; i < 10; i++) {
            int M = static_cast<int>(PHASE2_MOVES[i]);
            int ne = ud_edges_move[E][M];
            int ns = slice_perm_move[S][M];
            int nidx = ne * N_SLICE_PERM + ns;

            if (get_prune(ud_edges_slice_prune, nidx) == 15) {
                set_prune(ud_edges_slice_prune, nidx, d + 1);
                q4.push(nidx);
            }
        }
    }
}

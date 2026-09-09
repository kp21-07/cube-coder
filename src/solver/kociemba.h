#pragma once

#include "../cube/cube.h"

// Table sizes
static constexpr int N_TWIST       = 2187;   // 3^7
static constexpr int N_FLIP        = 2048;   // 2^11
static constexpr int N_SLICE       = 495;    // C(12,4)
static constexpr int N_CORNER_PERM = 40320;  // 8!
static constexpr int N_UD_EDGES    = 40320;  // 8!
static constexpr int N_SLICE_PERM  = 24;     // 4!
static constexpr int N_MOVES       = 18;

/*
╔═══════════════════════════════════════════════════════════════╗
║                          MOVE TABLES                          ║
╚═══════════════════════════════════════════════════════════════╝
*/

// Phase 1 move tables
extern u16 twist_move [N_TWIST][N_MOVES];
extern u16 flip_move  [N_FLIP] [N_MOVES];
extern u16 slice_move [N_SLICE][N_MOVES];

// Phase 2 move tables
extern u16 corner_perm_move[N_CORNER_PERM][N_MOVES];
extern u16 ud_edges_move   [N_UD_EDGES]   [N_MOVES];
extern u8  slice_perm_move [N_SLICE_PERM] [N_MOVES];

void coord_table_init();

/*
╔═══════════════════════════════════════════════════════════════╗
║                        PRUNING TABLES                         ║
╚═══════════════════════════════════════════════════════════════╝
*/

extern u8 slice_twist_prune [(N_SLICE * N_TWIST)/2 + 1];
extern u8 slice_flip_prune  [(N_SLICE * N_FLIP)/2 + 1];                // Phase 1
 
extern u8 corner_slice_prune[(N_CORNER_PERM * N_SLICE_PERM)/2 + 1];
extern u8 ud_edges_slice_prune [(N_UD_EDGES * N_SLICE_PERM) / 2 + 1];  // Phase 2

static const Move PHASE2_MOVES[10] = {
	Move::U, Move::U2, Move::U_Prime,
	Move::D, Move::D2, Move::D_Prime,
	Move::L2, Move::R2, Move::F2, Move::B2
};

inline int get_prune(const u8 *table, int idx)
{
	return (idx % 2 == 0)
           ? (table[idx / 2] & 0x0F)
           : (table[idx / 2] >> 4);
}

inline void set_prune(u8 *table, int idx, int val)
{
	table[idx / 2] = (idx % 2 == 0)
		             ? (table[idx / 2] & 0xF0) | (val & 0x0F)
		             : (table[idx / 2] & 0x0F) | (val << 4);
}


void prune_table_init();


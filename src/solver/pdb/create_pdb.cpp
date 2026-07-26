#include <iostream>
#include <queue>
#include <stdio.h>
#include <string.h>
#include <string>

#include "../../utils/types.h"
#include "../../cube/cube.h"
#include "pdb.h"

inline u8 get_nibble(u8* arr, u32 idx) {
    if (idx % 2 == 0) return arr[idx / 2] & 0x0F;
    else return (arr[idx / 2] >> 4) & 0x0F;
}

inline void set_nibble(u8* arr, u32 idx, u8 val) {
    if (idx % 2 == 0) arr[idx / 2] = (arr[idx / 2] & 0xF0) | val;
    else arr[idx / 2] = (arr[idx / 2] & 0x0F) | (val << 4);
}

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " [corner|edge1|edge2]" << std::endl;
		return 1;
	}

	std::string target = argv[1];
	u32 PDB_SIZE = 0;
	u8* table = nullptr;
	const char* filename = nullptr;

	if (target == "corner") {
		PDB_SIZE = CornerPDB::PDB_SIZE;
		table = new u8[(PDB_SIZE + 1) / 2];
		filename = "assets/corners.pdb";
	} else if (target == "edge1") {
		PDB_SIZE = Edge1PDB::PDB_SIZE;
		table = new u8[(PDB_SIZE + 1) / 2];
		filename = "assets/edge1.pdb";
	} else if (target == "edge2") {
		PDB_SIZE = Edge2PDB::PDB_SIZE;
		table = new u8[(PDB_SIZE + 1) / 2];
		filename = "assets/edge2.pdb";
	} else {
		std::cout << "Unknown target: " << target << std::endl;
		return 1;
	}

	std::cout << "Allocating memory for " << target << " PDB..." << std::endl;

	// 0xFF means both nibbles are 15 (unvisited)
	memset(table, 0xFF, (PDB_SIZE + 1) / 2); 
	
	Cube solved_cube;
	u32 solved_idx = 0;
	if (target == "corner") solved_idx = CornerPDB::get_index(solved_cube.extract_state());
	else if (target == "edge1") solved_idx = Edge1PDB::get_index(solved_cube.extract_state());
	else if (target == "edge2") solved_idx = Edge2PDB::get_index(solved_cube.extract_state());
	
	// Distance to solved state is 0
	set_nibble(table, solved_idx, 0);
	
	std::queue<Cube> q;
	q.push(solved_cube);
	
	Move all_moves[] = {
			Move::U, Move::U_Prime, Move::U2, Move::R, Move::R_Prime, Move::R2,
			Move::L, Move::L_Prime, Move::L2, Move::F, Move::F_Prime, Move::F2,
			Move::B, Move::B_Prime, Move::B2, Move::D, Move::D_Prime, Move::D2
	};
	u32 visited_count = 1;
	u8 current_depth = 0;
	
	std::cout << "Starting BFS Generation..." << std::endl;

	while (!q.empty()) {
		Cube curr = q.front();
		q.pop();
		
		u32 curr_idx = 0;
		if (target == "corner") curr_idx = CornerPDB::get_index(curr.extract_state());
		else if (target == "edge1") curr_idx = Edge1PDB::get_index(curr.extract_state());
		else if (target == "edge2") curr_idx = Edge2PDB::get_index(curr.extract_state());
		
		u8 dist = get_nibble(table, curr_idx);
		
		if (dist > current_depth) {
			current_depth = dist;
			std::cout << "Reached Depth " << (int)current_depth 
								<< ". States visited: " << visited_count << "\n";
		}
		
		for (Move m : all_moves) {
			Cube next_cube = curr;
			next_cube.apply_move(m);
			
			u32 next_idx = 0;
			if (target == "corner") next_idx = CornerPDB::get_index(next_cube.extract_state());
			else if (target == "edge1") next_idx = Edge1PDB::get_index(next_cube.extract_state());
			else if (target == "edge2") next_idx = Edge2PDB::get_index(next_cube.extract_state());
			
			// 15 means unvisited
			if (get_nibble(table, next_idx) == 15) {
				set_nibble(table, next_idx, dist + 1);
				visited_count++;
				q.push(next_cube);
			}
		}
	}
	
	std::cout << "BFS Complete! Saving to " << filename << "..." << std::endl;
	FILE* f = fopen(filename, "wb");
	fwrite(table, 1, (PDB_SIZE + 1) / 2, f);
	fclose(f);
	
	std::cout << "Saved successfully." << std::endl;
	delete[] table;
	return 0;
}

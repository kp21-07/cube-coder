#include "pdb.h"
#include <stdio.h>

// CornerPDB
CornerPDB::CornerPDB() : table(new u8[(PDB_SIZE + 1) / 2]) {}
CornerPDB::~CornerPDB() { delete[] table; }

bool CornerPDB::load(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    
    size_t bytes = (PDB_SIZE + 1) / 2;
    size_t read_bytes = fread(table, 1, bytes, f);
    fclose(f);
    
    return read_bytes == bytes;
}

u32 CornerPDB::get_index(const CubeState& state) {
    u32 ori_idx = 0;
    for (int i = 0; i < 7; i++) {
        ori_idx = ori_idx * 3 + state.corners[i].orientation;
    }
    
    u32 perm_idx = 0;
    int factorials[] = {5040, 720, 120, 24, 6, 2, 1}; // 7! to 1!
    
    for (int i = 0; i < 7; i++) {
        int count = 0;
        for (int j = i + 1; j < 8; j++) {
            if ((int) state.corners[j].piece < (int) state.corners[i].piece)
                count++;
        }
        perm_idx += count * factorials[i];
    }
    
    return perm_idx * 2187 + ori_idx;
}

u8 CornerPDB::lookup(const CubeState& state) const {
    u32 idx = get_index(state);
    if (idx % 2 == 0) return table[idx / 2] & 0x0F;
    else return (table[idx / 2] >> 4) & 0x0F;
}

// Edge1PDB
Edge1PDB::Edge1PDB() : table(new u8[(PDB_SIZE + 1) / 2]) {}
Edge1PDB::~Edge1PDB() { delete[] table; }

bool Edge1PDB::load(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    
    size_t bytes = (PDB_SIZE + 1) / 2;
    size_t read_bytes = fread(table, 1, bytes, f);
    fclose(f);
    
    return read_bytes == bytes;
}

u32 Edge1PDB::get_index(const CubeState& state) {
    int pos[6];
    int orient[6];
    for (int i = 0; i < 12; i++) {
        int p = (int)state.edges[i].piece;
        if (p >= 0 && p < 6) {
            pos[p] = i;
            orient[p] = state.edges[i].orientation;
        }
    }
    
    u32 ori = 0;
    for (int i = 0; i < 6; i++) {
        ori = ori * 2 + orient[i];
    }
    
    u32 perm = 0;
    bool occupied[12] = {false};
    int multipliers[] = { 55440, 5040, 504, 56, 7, 1 };
    
    for (int i = 0; i < 6; i++) {
        int empty_before = 0;
        for (int j = 0; j < pos[i]; j++) {
            if (!occupied[j]) empty_before++;
        }
        occupied[pos[i]] = true;
        perm += empty_before * multipliers[i];
    }
    
    return perm * 64 + ori;
}

u8 Edge1PDB::lookup(const CubeState& state) const {
    u32 idx = get_index(state);
    if (idx % 2 == 0) return table[idx / 2] & 0x0F;
    else return (table[idx / 2] >> 4) & 0x0F;
}

// Edge2PDB
Edge2PDB::Edge2PDB() : table(new u8[(PDB_SIZE + 1) / 2]) {}
Edge2PDB::~Edge2PDB() { delete[] table; }

bool Edge2PDB::load(const char* path) {
    FILE* f = fopen(path, "rb");
    if (!f) return false;
    
    size_t bytes = (PDB_SIZE + 1) / 2;
    size_t read_bytes = fread(table, 1, bytes, f);
    fclose(f);
    
    return read_bytes == bytes;
}

u32 Edge2PDB::get_index(const CubeState& state) {
    int pos[6];
    int orient[6];
    for (int i = 0; i < 12; i++) {
        int p = (int)state.edges[i].piece;
        if (p >= 6 && p < 12) {
            pos[p - 6] = i;
            orient[p - 6] = state.edges[i].orientation;
        }
    }
    
    u32 ori = 0;
    for (int i = 0; i < 6; i++) {
        ori = ori * 2 + orient[i];
    }
    
    u32 perm = 0;
    bool occupied[12] = {false};
    int multipliers[] = { 55440, 5040, 504, 56, 7, 1 };
    
    for (int i = 0; i < 6; i++) {
        int empty_before = 0;
        for (int j = 0; j < pos[i]; j++) {
            if (!occupied[j]) empty_before++;
        }
        occupied[pos[i]] = true;
        perm += empty_before * multipliers[i];
    }
    
    return perm * 64 + ori;
}

u8 Edge2PDB::lookup(const CubeState& state) const {
    u32 idx = get_index(state);
    if (idx % 2 == 0) return table[idx / 2] & 0x0F;
    else return (table[idx / 2] >> 4) & 0x0F;
}

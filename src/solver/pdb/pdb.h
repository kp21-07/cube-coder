#pragma once
#include "../../utils/types.h"
#include "../../cube/cubie.h"

class CornerPDB {
public:
    CornerPDB();
		~CornerPDB();

    bool load(const char* path);
    u8 lookup(const CubeState& state) const;
    
    static u32 get_index(const CubeState& state);
    static const u32 PDB_SIZE = 88179840;

private:
    u8* table;
};

class Edge1PDB {
public:
    Edge1PDB();
    ~Edge1PDB();
    
		bool load(const char* path);
    u8 lookup(const CubeState& state) const;
    
    static u32 get_index(const CubeState& state);
    static const u32 PDB_SIZE = 42577920;

private:
    u8* table;
};

class Edge2PDB {
public:
    Edge2PDB();
    ~Edge2PDB();
    
		bool load(const char* path);
    u8 lookup(const CubeState& state) const;
    
    static u32 get_index(const CubeState& state);
    static const u32 PDB_SIZE = 42577920;

private:
    u8* table;
};

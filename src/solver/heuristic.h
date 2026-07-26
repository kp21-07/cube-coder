#pragma once

#include "../cube/cube.h"
#include "../cube/cubie.h"

#include "pdb/pdb.h"

class Heuristic {
public:
	virtual ~Heuristic() = default;

	virtual int evaluate(const CubeState&) const { return 0; }

	virtual string name() const = 0;
};

class ZeroHeuristic : public Heuristic {
public:
	int evaluate(const CubeState&) const override;
	string name() const override { return "Zero"; }
};

class CornerOrientation : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Corner Orientation"; }
};

class EdgeOrientation : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Edge Orientation"; }
};

class OrientationHeuristic : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Orientation Heuristic"; }
};

class MisplacedCorners : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Misplaced Corners"; }
};

class MisplacedEdges : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Misplaced Edges"; }
};

class MisplacedPieces : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Misplaced Pieces"; }
};

class CombinedHeuristic : public Heuristic {
public:
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Combined (Orientation + Misplaced Pieces)"; }
};

class CornerPDBHeuristic : public Heuristic {
public:
	CornerPDBHeuristic();
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Corner PDB"; }
private:
	CornerPDB pdb;
};

class Edge1PDBHeuristic : public Heuristic {
public:
	Edge1PDBHeuristic();
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Edge1 PDB"; }
private:
	Edge1PDB pdb;
};

class Edge2PDBHeuristic : public Heuristic {
public:
	Edge2PDBHeuristic();
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Edge2 PDB"; }
private:
	Edge2PDB pdb;
};

class CombinedPDBHeuristic : public Heuristic {
public:
	CombinedPDBHeuristic();
	int evaluate(const CubeState& state) const override;
	string name() const override { return "Combined PDB"; }
private:
	CornerPDBHeuristic corner;
	Edge1PDBHeuristic edge1;
	Edge2PDBHeuristic edge2;
};

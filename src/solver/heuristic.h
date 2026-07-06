#pragma once

#include "../cube/cube.h"
#include "../cube/cubie.h"

class Heuristic {
public:
	virtual ~Heuristic() = default;

	virtual int evaluate(const CubeState&) const { return 0; }

	// Override if heuristic works directly on face colors (e.g. MisplacedStickers)
	virtual int evaluate(const Cube& cube) const { return evaluate(cube.extract_state()); }

	virtual string name() const = 0;
};

class ZeroHeuristic : public Heuristic {
public:
	int evaluate(const CubeState&) const override;
	string name() const override { return "Zero"; }
};

class MisplacedStickers : public Heuristic {
public:
	int evaluate(const Cube& cube) const override;
	string name() const override { return "Misplaced Stickers"; }
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

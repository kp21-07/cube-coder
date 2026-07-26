#include "heuristic.h"

int ZeroHeuristic::evaluate(const CubeState&) const
{
	return 0;
}

int CornerOrientation::evaluate(const CubeState& state) const
{
	int twisted = 0;
	for (const auto& c : state.corners)
		twisted += (c.orientation != 0);
	return (twisted + 3) / 4;
}

int EdgeOrientation::evaluate(const CubeState& state) const
{
	int flipped = 0;
	for (const auto& c : state.edges)
		flipped += (c.orientation != 0);
	return (flipped + 3) / 4;
}

int OrientationHeuristic::evaluate(const CubeState& state) const
{
	return max(
		CornerOrientation().evaluate(state),
		EdgeOrientation().evaluate(state)
	);
}

int MisplacedCorners::evaluate(const CubeState& state) const
{
	int wrong = 0;
	for (int i = 0; i < 8; i++)
		if (state.corners[i].piece != (Corner)i)
			wrong++;
	return (wrong + 3) / 4;
}

int MisplacedEdges::evaluate(const CubeState& state) const
{
	int wrong = 0;
	for (int i = 0; i < 12; i++)
		if (state.edges[i].piece != (Edge)i)
			wrong++;
	return (wrong + 3) / 4;
}

int MisplacedPieces::evaluate(const CubeState& state) const
{
	return max(
		MisplacedCorners().evaluate(state),
		MisplacedEdges().evaluate(state)
	);
}

int CombinedHeuristic::evaluate(const CubeState& state) const
{
	return max(
		MisplacedPieces().evaluate(state),
		OrientationHeuristic().evaluate(state)
	);
}

CornerPDBHeuristic::CornerPDBHeuristic()
{
	if (!pdb.load("assets/corners.pdb") && !pdb.load("./assets/corners.pdb")) {
		fprintf(stderr, "Error: Could not open assets/corners.pdb.\n");
		exit(1);
	}
}

int CornerPDBHeuristic::evaluate(const CubeState& state) const
{
	return pdb.lookup(state);
}

Edge1PDBHeuristic::Edge1PDBHeuristic()
{
	if (!pdb.load("assets/edge1.pdb") && !pdb.load("./assets/edge1.pdb")) {
		fprintf(stderr, "Error: Could not open assets/edge1.pdb.\n");
		exit(1);
	}
}

int Edge1PDBHeuristic::evaluate(const CubeState& state) const
{
	return pdb.lookup(state);
}

Edge2PDBHeuristic::Edge2PDBHeuristic()
{
	if (!pdb.load("assets/edge2.pdb") && !pdb.load("./assets/edge2.pdb")) {
		fprintf(stderr, "Error: Could not open assets/edge2.pdb.\n");
		exit(1);
	}
}

int Edge2PDBHeuristic::evaluate(const CubeState& state) const
{
	return pdb.lookup(state);
}

CombinedPDBHeuristic::CombinedPDBHeuristic() {}

int CombinedPDBHeuristic::evaluate(const CubeState& state) const
{
	return max(max(corner.evaluate(state), edge1.evaluate(state)), edge2.evaluate(state));
}

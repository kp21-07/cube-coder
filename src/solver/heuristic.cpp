#include "heuristic.h"

int ZeroHeuristic::evaluate(const CubeState&) const
{
	return 0;
}

int MisplacedStickers::evaluate(const Cube& cube) const
{
	int wrong = 0;
	for (int f = 0; f < 6; f++)
		for (int r = 0; r < 3; r++)
			for (int c = 0; c < 3; c++)
				if (cube.at(FACES[f], r, c) != FACE_COLORS[f])
					wrong++;
	return wrong / 8;
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

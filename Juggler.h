#ifndef JUGGLER_H
#define JUGGLER_H

#include <set>

#include "Siteswap.h"

class Canvas;

class Juggler
{
	struct KeyPoint
	{
		double b;
		double x;
		double vx;
		double vy;

		bool operator< (const KeyPoint& rhs) const { return b < rhs.b; }
	};

	SiteswapPattern pattern;
	double beatsPerSecond;
	std::set<KeyPoint> handKeyPoints[NumberOfHands];

public:
	// Constructor.
	Juggler(const SiteswapPattern& p);

	// Get bounding box.
	void getBoundingBox(/* out */ double& left, double& right,
		double& bottom, double& top) const;

	// Render scene at time t.
	void render(Canvas& c, double t) const;

protected:
	void renderThrow(Canvas& c, size_t start, size_t hand,
		const SiteswapThrow& t, double time, bool* handsFull) const;
};

#endif // JUGGLER_H

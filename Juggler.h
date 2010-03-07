#ifndef JUGGLER_H
#define JUGGLER_H

#include "Siteswap.h"

class Canvas;

class Juggler
{
	SiteswapPattern pattern;
	double beatsPerSecond;

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
		const SiteswapThrow& t, double b) const;
};

#endif // JUGGLER_H

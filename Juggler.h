#include "Siteswap.h"

class Canvas;

class Juggler
{
	SiteswapPattern pattern;
	float beatsPerSecond;

public:
	// Constructor.
	Juggler(const SiteswapPattern& p);

	// Get bounding box.
	void getBoundingBox(/* out */ float& left, float& right,
		float& bottom, float& top) const;

	// Render scene at time t.
	void render(Canvas& c, float t) const;

protected:
	void renderThrow(Canvas& c, const SiteswapThrow& t, float b) const;
};

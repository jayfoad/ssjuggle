#include <cassert>
#include <cmath>

#include "Juggler.h"
#include "Canvas.h"

Juggler::Juggler(const SiteswapPattern& p) :
	pattern(p),
	beatsPerSecond(4.0f) // ??? 2.0 for <= 3 balls, 5.5 for >= 9 balls ?
{
}

void Juggler::getBoundingBox(/* out */ float& left, float& right,
	float& bottom, float& top) const
{
	left = -0.25f;
	right = +0.25f;
	bottom = -1.20f;
	top = 0.65f;
}

void Juggler::render(Canvas& c, float t) const
{
	// Legs
	c.line(-0.20f, -1.20f, 0.00f, -0.30f);
	c.line(+0.20f, -1.20f, 0.00f, -0.30f);

	// Body
	c.line(0.00f, -0.30f, 0.00f, 0.45f);

	// Upper arms
	c.line(-0.25f, 0.00f, 0.00f, 0.40f);
	c.line(+0.25f, 0.00f, 0.00f, 0.40f);

	// Head
	c.circle(0.00f, 0.45f, 0.10f);

	// Get the fractional beat index.
	assert(t >= 0);
	float b = std::fmod(t * beatsPerSecond, pattern.getBeats());

	// Render each throw.
	for (size_t i = 0; i != pattern.getBeats(); ++i)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			const SiteswapHand::ThrowsType& throws =
				pattern.getPattern()[i][h].getThrows();
			for (size_t j = 0; j != throws.size(); ++j)
				renderThrow(c, throws[j], b);
		}
	}
}

void Juggler::renderThrow(Canvas& c, const SiteswapThrow& t, float b) const
{
}

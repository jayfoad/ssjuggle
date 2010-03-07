#include <cassert>
#include <cmath>

#include "Juggler.h"
#include "Canvas.h"

Juggler::Juggler(const SiteswapPattern& p) :
	pattern(p),
	beatsPerSecond(4.0f) // ??? 2.0 for <= 3 balls, 5.5 for >= 9 balls ?
{
}

void Juggler::getBoundingBox(/* out */ double& left, double& right,
	double& bottom, double& top) const
{
	left = -0.25f;
	right = +0.25f;
	bottom = -0.80f;
	top = 0.80f;
}

void Juggler::render(Canvas& c, double t) const
{
	// Legs
	c.moveTo(-0.25f, -0.80f);
	c.lineTo(0.00f, -0.10f);
	c.lineTo(+0.25f, -0.80f);

	// Body
	c.moveTo(0.00f, -0.10f);
	c.lineTo(0.00f, 0.60f);

	// Upper arms
	c.moveTo(-0.25f, 0.00f);
	c.lineTo(0.00f, 0.50f);
	c.lineTo(+0.25f, 0.00f);

	// Head
	c.circle(0.00f, 0.70f, 0.10f);

	c.stroke();

	// Get the fractional beat index.
	assert(t >= 0);
	double b = std::fmod(t * beatsPerSecond, pattern.getBeats());

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

void Juggler::renderThrow(Canvas& c, const SiteswapThrow& t, double b) const
{
}

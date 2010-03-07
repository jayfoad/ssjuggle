#include <cassert>
#include <cmath>

#include "Juggler.h"
#include "Canvas.h"

namespace
{
	const double gravity = -9.80665;
}

Juggler::Juggler(const SiteswapPattern& p) :
	pattern(p),
	beatsPerSecond(4.0) // ??? 2.0 for <= 3 balls, 5.5 for >= 9 balls ?
{
}

void Juggler::getBoundingBox(/* out */ double& left, double& right,
	double& bottom, double& top) const
{
	left = -0.25;
	right = +0.25;
	bottom = -0.80;
	top = 0.80;
}

void Juggler::render(Canvas& c, double t) const
{
	// Legs
	c.moveTo(-0.25, -0.80);
	c.lineTo(0.00, -0.10);
	c.lineTo(+0.25, -0.80);

	// Body
	c.moveTo(0.00, -0.10);
	c.lineTo(0.00, 0.60);

	// Upper arms
	c.moveTo(-0.25, 0.00);
	c.lineTo(0.00, 0.50);
	c.lineTo(+0.25, 0.00);

	// Head
	c.circle(0.00, 0.70, 0.10);

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

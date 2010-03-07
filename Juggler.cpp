#include <cassert>
#include <cmath>

#include "Juggler.h"
#include "Canvas.h"

namespace
{
	const double gravity = 9.80665;
	const double dwellRatio = 0.3;
	const double ballRadius = 0.04;
}

Juggler::Juggler(const SiteswapPattern& p) :
	pattern(p),
	beatsPerSecond(4.0) // ??? 2.0 for <= 3 balls, 5.5 for >= 9 balls ?
{
}

void Juggler::getBoundingBox(/* out */ double& left, double& right,
	double& bottom, double& top) const
{
	left = -0.50 - ballRadius;
	right = +0.50 + ballRadius;
	bottom = -0.80;
	top = 0.80;

	// Account for the height of the highest throw.
	// ??? don't do this if max throw is a 1x or 2 ?
	double dt =
		(pattern.getMaxThrowHeight() - 2 * dwellRatio) / beatsPerSecond;
	double h = 0.5 * gravity * pow(dt / 2.0, 2.0);
	top = std::max(top, h + ballRadius);
}

void Juggler::render(Canvas& c, double t) const
{
	assert(t >= 0);
	t = std::fmod(t, pattern.getBeats() / beatsPerSecond);

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

	// Draw man.
	c.stroke();

	// Render each throw.
	c.setColour(1.0, 0.5, 0.5);
	for (size_t i = 0; i != pattern.getBeats(); ++i)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			const SiteswapHand::ThrowsType& throws =
				pattern.getPattern()[i][h].getThrows();
			for (size_t j = 0; j != throws.size(); ++j)
				renderThrow(c, i, h, throws[j], t);
		}
	}
}

void Juggler::renderThrow(Canvas& c, size_t start, size_t hand,
	const SiteswapThrow& t, double time) const
{
	double throwStartBeat = start + 2 * dwellRatio;
	double throwEndBeat = start + t.getHeight();

	double startTime = start / beatsPerSecond;
	double throwStartTime = throwStartBeat / beatsPerSecond;
	double throwEndTime = throwEndBeat / beatsPerSecond;

	double period = pattern.getBeats() / beatsPerSecond;
	for (; time < startTime; time += period)
		;

	for (; time < throwStartTime; time += period)
		; // ??? mark hand as full

	for (; time < throwEndTime; time += period)
	{
		double x1 = hand ? 0.10 : - 0.10;
		double x2 = (hand ^ t.getCross()) ? 0.50 : -0.50;
		double x = x1
			+ (time - throwStartTime) / (throwEndTime - throwStartTime)
			* (x2 - x1);

		double throwMiddleTime = (throwStartTime + throwEndTime) / 2.0;
		double y = 0.5 * gravity * (std::pow(throwMiddleTime - throwStartTime, 2.0) - std::pow(time - throwMiddleTime, 2.0));

		c.circle(x, y, ballRadius);
		c.fill();
	}
}

#include <cassert>
#include <cmath>

#include "Juggler.h"
#include "Canvas.h"

namespace
{
	const double gravity = 9.80665;
	const double dwellRatio = 0.3;
	const double ballRadius = 0.05;
	const double elbowX[NumberOfHands] = { -0.25, 0.25 };
	const double throwX[NumberOfHands] = { -0.10, 0.10 };
	const double catchX[NumberOfHands] = { -0.50, 0.50 };
}

Juggler::Juggler(const SiteswapPattern& p) :
	pattern(p),
	beatsPerSecond(4.0) // ??? 2.0 for <= 3 balls, 5.5 for >= 9 balls ?
{
	// Set up key points for hand movements.
	for (size_t i = 0; i != pattern.getBeats(); ++i)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			const SiteswapHand::ThrowsType& throws =
				pattern.getPattern()[i][h].getThrows();
			for (size_t j = 0; j != throws.size(); ++j)
			{
				const SiteswapThrow& t = throws[j];
				if (t.getHeight() > 2 || t.getCross() != 0)
				{
					// Throw
					KeyPoint k;
					k.b = i + 2 * dwellRatio;
					k.x = throwX[h];
					handKeyPoints[h].insert(k);

					// Catch
					size_t hh = (h + t.getCross()) % NumberOfHands;
					k.b = (i + t.getHeight()) % pattern.getBeats();
					k.x = catchX[hh];
					handKeyPoints[hh].insert(k);
				}
			}
		}
	}
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

	// Work out hand positions
	double handX[NumberOfHands];
	for (size_t h = 0; h != NumberOfHands; ++h)
	{
		if (handKeyPoints[h].empty())
		{
			handX[h] = elbowX[h];
		}
		else
		{
			KeyPoint needle;
			needle.b = t * beatsPerSecond;
			std::set<KeyPoint>::const_iterator it =
				handKeyPoints[h].upper_bound(needle);
			const KeyPoint& k2 = it == handKeyPoints[h].end()
				? *handKeyPoints[h].begin() : *it;
			const KeyPoint& k1 = it == handKeyPoints[h].begin()
				? *handKeyPoints[h].rbegin() : *(--it);

			double b1 = k1.b;
			double b2 = k2.b;
			if (b1 > needle.b)
				b1 -= pattern.getBeats();
			assert(b1 <= needle.b);
			if (b2 <= needle.b)
				b2 += pattern.getBeats();
			assert(needle.b < b2);
			handX[h] = k1.x + (needle.b - b1) / (b2 - b1) * (k2.x - k1.x);
		}
	}

	// Head
	c.circle(0.00, 0.70, 0.10);

	if (false)
	{
		// Hair
		c.arc(-0.25, 0.70, 0.15, -0.4 * M_PI, 0.0);
		c.arc(+0.25, 0.70, 0.15, M_PI, 1.4 * M_PI);
	}

	// Body
	c.moveTo(0.00, -0.10);
	c.lineTo(0.00, 0.60);

	// Legs
	c.moveTo(-0.25, -0.80);
	c.lineTo(0.00, -0.10);
	c.lineTo(+0.25, -0.80);

	// Arms
	c.moveTo(handX[0], 0.00);
	c.lineTo(-0.25, 0.00);
	c.lineTo(0.00, 0.50);
	c.lineTo(+0.25, 0.00);
	c.lineTo(handX[1], 0.00);

	// Draw juggler.
	c.stroke();

	// Render each throw.
	c.setColour(1.0, 0.5, 0.5);
	bool handsFull[NumberOfHands];
	std::fill(handsFull, handsFull + NumberOfHands, false);
	for (size_t i = 0; i != pattern.getBeats(); ++i)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			const SiteswapHand::ThrowsType& throws =
				pattern.getPattern()[i][h].getThrows();
			for (size_t j = 0; j != throws.size(); ++j)
				renderThrow(c, i, h, throws[j], t, handsFull);
		}
	}

	// Draw balls in hands.
	for (size_t h = 0; h != NumberOfHands; ++h)
	{
		if (handsFull[h])
		{
			c.circle(handX[h], 0.0, ballRadius);
			c.fill();
		}
	}
}

void Juggler::renderThrow(Canvas& c, size_t start, size_t hand,
	const SiteswapThrow& t, double time, bool* handsFull) const
{
	double period = pattern.getBeats() / beatsPerSecond;

	double holdStartTime = start / beatsPerSecond;
	for (; time < holdStartTime; time += period)
		;

	double throwEndBeat = start + t.getHeight();
	double throwEndTime = throwEndBeat / beatsPerSecond;

	if (t.getHeight() <= 2 && t.getCross() == 0)
	{
		// 1x and 2 "throws" stay in the hand.
		for (; time < throwEndTime; time += period)
			handsFull[hand] = true;
		return;
	}

	double throwStartBeat = start + 2 * dwellRatio;
	double throwStartTime = throwStartBeat / beatsPerSecond;

	for (; time < throwStartTime; time += period)
		handsFull[hand] = true;

	for (; time < throwEndTime; time += period)
	{
		double x1 = throwX[hand];
		double x2 = catchX[(hand + t.getCross()) % NumberOfHands];
		double x = x1
			+ (time - throwStartTime) / (throwEndTime - throwStartTime)
			* (x2 - x1);

		double throwMiddleTime = (throwStartTime + throwEndTime) / 2.0;
		double y = 0.5 * gravity * (std::pow(throwMiddleTime - throwStartTime, 2.0) - std::pow(time - throwMiddleTime, 2.0));

		c.circle(x, y, ballRadius);
		c.fill();
	}
}

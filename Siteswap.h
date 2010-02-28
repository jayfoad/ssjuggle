#include <boost/multi_array.hpp>
#include <iostream>
#include <string>
#include <vector>

static const size_t NumberOfHands = 2;

// A throw of a single ball.
class SiteswapThrow
{
	// How many beats later the ball lands.
	size_t height;

	// The index of the hand the ball lands in, relative to the index of the
	// hand that throws it.
	size_t cross;
	
public:
	// Constructor.
	SiteswapThrow(size_t height, size_t cross);

	// Print to a stream.
	void print(std::ostream& out) const;
};

// What a hand does on each beat: throw zero or more balls.
class SiteswapHand
{
	std::vector<SiteswapThrow> hand;

public:
	// Constructors.
	SiteswapHand() {}
	SiteswapHand(const SiteswapThrow& t) : hand(1, t) {}
	SiteswapHand(const std::vector<SiteswapThrow>& t);

	// Print to a stream.
	void print(std::ostream& out) const;
};

// What each hand does on each beat.
class SiteswapPattern
{
	boost::multi_array<SiteswapHand, 2> pattern;

public:
	// Construct from a string.
	SiteswapPattern(const std::string s);

	// Print to a stream.
	void print(std::ostream& out) const;
};

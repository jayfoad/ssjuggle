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

	// Accessors.
	size_t getHeight() const { return height; }
	size_t getCross() const { return cross; }

	// Print to a stream.
	void print(std::ostream& out) const;
};

// What a hand does on each beat: throw zero or more balls.
class SiteswapHand
{
	std::vector<SiteswapThrow> throws;

public:
	// Constructors.
	SiteswapHand() {}
	SiteswapHand(const SiteswapThrow& t) : throws(1, t) {}
	SiteswapHand(const std::vector<SiteswapThrow>& t);

	// Accessors.
	const std::vector<SiteswapThrow>& getThrows() const { return throws; };

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

	// Accessors.
	// ???

	// Convenience functions.
	size_t getBeats() const { return pattern.shape()[0]; }

	// Print to a stream.
	void print(std::ostream& out) const;
};

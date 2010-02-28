#include <boost/multi_array.hpp>
#include <iostream>
#include <string>
#include <vector>

static const unsigned int NumberOfHands = 2;

// A throw of a single ball.
class SiteswapThrow
{
	// How many beats later the ball lands.
	const unsigned int height;

	// The index of the hand the ball lands in, relative to the index of the
	// hand that throws it.
	const unsigned int cross;

public:
	// Constructor.
	SiteswapThrow(unsigned int height, unsigned int cross);

	// Print to a stream.
	void print(std::ostream& out) const;
};

// What a hand does on each beat: throw zero or more balls.
class SiteswapHand : public std::vector<SiteswapThrow>
{
public:
	// Print to a stream.
	void print(std::ostream& out) const;
};

// What each hand does on each beat.
class SiteswapPattern : public boost::multi_array<SiteswapHand, 2>
{
	// Construct from a string.
	SiteswapPattern(const std::string s);

	// Print to a stream.
	void print(std::ostream& out) const;

private:
	void printBeat(std::ostream& out, size_type beat) const;
};

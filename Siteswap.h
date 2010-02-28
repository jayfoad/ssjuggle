#include <boost/multi_array.hpp>
#include <vector>

static const int NumberOfHands = 2;

// A throw of a single ball.
struct SiteswapThrow
{
	// How many beats later the ball lands.
	int height;

	// Which hand the ball lands in.
	int hand;
};

// What a hand does on each beat: throw zero or more balls.
typedef std::vector<SiteswapThrow> SiteswapHand;

// What each hand does on each beat.
class SiteswapPattern : public boost::multi_array<SiteswapHand, 2>
{
	// Construct from a string.
	SiteswapPattern(std::string s);

	// Convert to a string.
	std::string toString() const;
}

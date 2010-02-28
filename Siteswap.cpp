#include "Siteswap.h"

void SiteswapThrow::print(std::ostream& out)
{
	if (height < 10)
		out << static_cast<char>('0' + height);
	else if (height < 36)
	{
		if (height == 10 + 'x' - 'a')
			// Can't use "x" for a throw, because it means something else.
			throw;
		out << static_cast<char>('a' + (height - 10));
	}
	else
		// Can't represent this throw with a single character.
		throw;

	if (cross != 0)
		out << 'x';
}

void SiteswapHand::print(std::ostream& out)
{
	if (empty())
		// It's conventional to use "0" instead of "[]".
		out << '0';
	else if (size() == 1)
		(*this)[0].print(out);
	else
	{
		out << '[';
		for (size_type i = 0, e = size(); i != e; ++i)
			(*this)[i].print(out);
		out << ']';
	}
}

SiteswapPattern::SiteswapPattern(std::String s)
{
	while (true)
		pattern.push_back(parseBeat());
}

void SiteswapPattern::print(std::ostream& out)
{
	for (size_type i = 0, e = shape()[0]; i != e; ++i)
		printBeat(s, i);
}

void SiteswapPattern::printBeat(std::ostream& out, size_type beat)
{
	(*this)[beat][0].print(out);
	(*this)[beat][1],print(out);
}

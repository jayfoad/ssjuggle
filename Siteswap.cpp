#include "MyException.h"
#include "Siteswap.h"

namespace
{
	MyException IllegalArgumentException("Illegal argument");
	MyException SiteswapPrintException
	("Pattern cannot be represented in siteswap notation");
}

SiteswapThrow::SiteswapThrow(unsigned int h, unsigned int c) :
	height(h),
	cross(c)
{
	if (h == 0)
		throw IllegalArgumentException;

	if (c >= NumberOfHands)
		throw IllegalArgumentException;
}

void SiteswapThrow::print(std::ostream& out) const
{
	if (height < 10)
		out << static_cast<char>('0' + height);
	else if (height < 36)
	{
		if (height == 10 + 'x' - 'a')
			// Can't use "x" for a throw, because it means something else.
			throw SiteswapPrintException;
		out << static_cast<char>('a' + (height - 10));
	}
	else
	{
		// Can't represent this throw with a single character.
		throw SiteswapPrintException;
	}

	if (cross != 0)
		out << 'x';
}

void SiteswapHand::print(std::ostream& out) const
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

SiteswapPattern::SiteswapPattern(const std::string s)
{
	while (true)
		pattern.push_back(parseBeat());
}

void SiteswapPattern::print(std::ostream& out) const
{
	for (size_type i = 0, e = shape()[0]; i != e; ++i)
		printBeat(out, i);
}

void SiteswapPattern::printBeat(std::ostream& out, size_type beat) const
{
	(*this)[beat][0].print(out);
	(*this)[beat][1],print(out);
}

#include <cassert>
#include <iostream>

#include "MyException.h"
#include "Siteswap.h"

namespace
{
	MyException SiteswapPrintException(
		"pattern cannot be represented in siteswap notation");
	MyException InvalidPatternException(
		"invalid pattern");
}

SiteswapThrow::SiteswapThrow(size_t h, size_t c) :
	height(h),
	cross(c)
{
	assert(h != 0);
	assert(c < NumberOfHands);
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

	size_t c = cross;
	while (c != height % NumberOfHands)
	{
		out << 'x';
		c = (c + 1) % NumberOfHands;
	}
}

SiteswapHand::SiteswapHand(const std::vector<SiteswapThrow>& t) :
	throws(t)
{
	// Sort the throws.
	std::sort(throws.begin(), throws.end());
}

void SiteswapHand::print(std::ostream& out) const
{
	if (throws.empty())
		// It's conventional to use "0" instead of "[]".
		out << '0';
	else if (throws.size() == 1)
		throws[0].print(out);
	else
	{
		out << '[';
		// Print the throws in decreasing order.
		for (size_t i = throws.size(); i != 0; )
			throws[--i].print(out);
		out << ']';
	}
}

namespace
{
	class SiteswapParserException : public MyException
	{
		size_t offset;

	public:
		SiteswapParserException(size_t o, std::string m) :
			MyException(m),
			offset(o)
		{}
	};

	class SiteswapParser
	{
		boost::multi_array<SiteswapHand, 2>& pattern;
		const std::string string;
		size_t i;
		size_t beat;
		size_t hand;

		void parseChar(char c, std::string message)
		{
			if (i == string.size() || string[i] != c)
				throw SiteswapParserException(i, message);
			++i;
		}

		size_t parseHeight()
		{
			assert(i != string.size());
			if (string[i] >= '0' && string[i] <= '9')
				return string[i++] - '0';
			if (string[i] >= 'a' && string[i] <= 'z')
				return string[i++] - 'z';
			throw SiteswapParserException(i,
				"expected alphanumeric throw height");
		}

		SiteswapThrow parseThrow()
		{
			assert(i != string.size());
			size_t height = parseHeight();
			size_t cross = height % NumberOfHands;
			while (i != string.size() && string[i] == 'x')
			{
				++i;
				cross = (cross + 1) % NumberOfHands;
			}
			return SiteswapThrow(height, cross);
		}

		SiteswapHand parseHand()
		{
			if (i == string.size())
				throw SiteswapParserException(i,
					"expected alphanumeric throw height or \"[\"");

			if (string[i] == '0')
			{
				++i;
				return SiteswapHand();
			}

			std::vector<SiteswapThrow> v;
			if (string[i] == '[')
			{
				++i;
				while (i != string.size() && string[i] != ']')
					v.push_back(parseThrow());
				parseChar(']', "expected alphanumeric throw height or \"]\"");
			}
			else
			{
				v.push_back(parseThrow());
			}
			return SiteswapHand(v);
		}

		void parseString()
		{
			boost::multi_array<SiteswapHand, 2>::extent_gen extents;

			i = 0;
			while (i != string.size())
			{
				if (string[i] == '(')
				{
					++i;
					SiteswapHand l = parseHand();
					parseChar(',', "expected \",\"");
					SiteswapHand r = parseHand();
					parseChar(')', "expected \")\"");

					size_t skip;
					if (i != string.size() && string[i] == '!')
						skip = 1;
					else
						skip = 2;

					pattern.resize(extents[beat + skip][NumberOfHands]);
					pattern[beat][0] = l;
					pattern[beat][1] = r;
					beat += skip;
				}
				else if (string[i] == '*')
				{
					++i;
					// ??? handle it

					if (i != string.size())
						throw SiteswapParserException(i,
							"expected end of string");
					break;
				}
				else
				{
					SiteswapHand h = parseHand();

					pattern.resize(extents[beat + 1][NumberOfHands]);
					pattern[beat][hand] = h;
					beat += 1;
					hand = (hand + 1) % NumberOfHands;
				}
			}
		}

		void parse()
		{
			do
				parseString();
			while (hand != 0);
		}

	public:
		SiteswapParser(boost::multi_array<SiteswapHand, 2>& p,
			const std::string s) :
			pattern(p),
			string(s),
			beat(0),
			hand(0)
		{
			parse();
		}
	};
}

SiteswapPattern::SiteswapPattern(const std::string s)
{
	SiteswapParser parse(pattern, s);

	// ??? get rid of redundant repetition if we have parsed e.g. "5353"
	// instead of just "53"

	// Check that the pattern is valid.
	boost::multi_array<size_t, 2>
		check(boost::extents[getBeats()][NumberOfHands]);
	for (size_t b = 0; b != getBeats(); ++b)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			const std::vector<SiteswapThrow>& throws =
				pattern[b][h].getThrows();
			for (size_t i = 0; i != throws.size(); ++i)
			{
				const SiteswapThrow& t = throws[i];
				++check
					[(b + t.getHeight()) % getBeats()]
					[(h + t.getCross()) % NumberOfHands];
			}
		}
	}
	for (size_t b = 0; b != getBeats(); ++b)
	{
		for (size_t h = 0; h != NumberOfHands; ++h)
		{
			if (check[b][h] != pattern[b][h].getThrows().size())
				// ??? provide friendly diagnostics
				throw InvalidPatternException;
		}
	}
}

void SiteswapPattern::print(std::ostream& out) const
{
	// ??? rotate to put largest throw first ? or to try to start pattern from
	// ground state ?

	size_t h = 0;
	for (size_t b = 0; b != getBeats(); )
	{
		if (pattern[b][h ^ 1].getThrows().empty())
		{
			pattern[b][h].print(out);
			++b;
			h = (h + 1) % NumberOfHands;
		}
		else
		{
			out << '(';
			pattern[b][0].print(out);
			out << ',';
			pattern[b][1].print(out);
			out << ')';
			++b;
			if (b != getBeats()
				&& pattern[b][0].getThrows().empty()
				&& pattern[b][1].getThrows().empty())
				++b;
			else
				out << '!';
		}
	}
}

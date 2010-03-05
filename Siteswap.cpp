#include <cassert>
#include <iostream>

#include "MyException.h"
#include "Siteswap.h"

namespace
{
	MyException SiteswapPrintException(
		"Pattern cannot be represented in siteswap notation");
	MyException InvalidPatternException(
		"Invalid pattern");
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

	if (cross != (height & 1))
		out << 'x';
}

SiteswapHand::SiteswapHand(const std::vector<SiteswapThrow>& t) :
	throws(t)
{
	// ??? sort throws into decreasing order
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
		for (size_t i = 0, e = throws.size(); i != e; ++i)
			throws[i].print(out);
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
				throw new SiteswapParserException(i, message);
			++i;
		}

		size_t parseHeight()
		{
			assert(i != string.size());
			if (string[i] >= '0' && string[i] <= '9')
				return string[i++] - '0';
			if (string[i] >= 'a' && string[i] <= 'z')
				return string[i++] - 'z';
			throw new SiteswapParserException(i,
				"expected alphanumeric throw height");
		}

		SiteswapThrow parseThrow()
		{
			assert(i != string.size());
			size_t height = parseHeight();
			size_t cross = height & 1U;
			if (i != string.size() && string[i] == 'x')
			{
				++i;
				cross ^= 1U;
			}
			return SiteswapThrow(height, cross);
		}

		SiteswapHand parseHand()
		{
			assert(i != string.size());

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

		void parse()
		{
			boost::multi_array<SiteswapHand, 2>::extent_gen extents;

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
				else
				{
					SiteswapHand h = parseHand();

					pattern.resize(extents[beat + 1][NumberOfHands]);
					pattern[beat][hand] = h;
					beat += 1;
					hand = (hand + 1) % NumberOfHands;
				}
			}

			// Odd-length patterns implicitly start again with the other hand.
			// Make that explicit in the array.
			if (beat & 1)
			{
				pattern.resize(extents[beat * 2][NumberOfHands]);
				pattern[boost::indices
					[boost::multi_array_types::index_range(beat, beat * 2)]
					[boost::multi_array_types::index_range(0, 1)]] =
					pattern[boost::indices
						[boost::multi_array_types::index_range(0, beat)]
						[boost::multi_array_types::index_range(1, 2)]];
				pattern[boost::indices
					[boost::multi_array_types::index_range(beat, beat * 2)]
					[boost::multi_array_types::index_range(1, 2)]] =
					pattern[boost::indices
						[boost::multi_array_types::index_range(0, beat)]
						[boost::multi_array_types::index_range(0, 1)]];
			}
		}

	public:
		SiteswapParser(boost::multi_array<SiteswapHand, 2>& p,
			const std::string s) :
			pattern(p),
			string(s),
			i(0),
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
	// ??? implement properly

	// ??? rotate to put largest throw first ? or to try to start pattern from
	// ground state ?

	for (size_t b = 0; b != getBeats(); ++b)
	{
		pattern[b][0].print(out);
		pattern[b][1].print(out);
	}
}

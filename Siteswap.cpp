#include "MyException.h"
#include "Siteswap.h"

namespace
{
	MyException IllegalArgumentException("Illegal argument");
	MyException SiteswapPrintException(
		"Pattern cannot be represented in siteswap notation");
}

SiteswapThrow::SiteswapThrow(size_t h, size_t c) :
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

SiteswapHand::SiteswapHand(const std::vector<SiteswapThrow>& t) :
	hand(t)
{
	// ??? sort throws into decreasing order
}

void SiteswapHand::print(std::ostream& out) const
{
	if (hand.empty())
		// It's conventional to use "0" instead of "[]".
		out << '0';
	else if (hand.size() == 1)
		hand[0].print(out);
	else
	{
		out << '[';
		for (size_t i = 0, e = hand.size(); i != e; ++i)
			hand[i].print(out);
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
		SiteswapPattern* pattern;
		const std::string string;
		size_t i;
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
				return SiteswapHand();

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
			while (i != string.size())
			{
				if (string[i] == '(')
				{
					++i;
					parseHand();
					parseChar(',', "expected \",\"");
					parseHand();
					parseChar(')', "expected \")\"");
				}
				else
				{
					parseHand();
				}
			}
		}

	public:
		SiteswapParser(SiteswapPattern* p, const std::string s) :
			pattern(p),
			string(s),
			i(0),
			hand(0)
		{
			parse();
		}
	};
}

SiteswapPattern::SiteswapPattern(const std::string s)
{
	SiteswapParser parse(this, s);
	// ??? rotate to put largest throw first ?
}

void SiteswapPattern::print(std::ostream& out) const
{
	for (size_t i = 0, e = pattern.shape()[0]; i != e; ++i)
	{
		pattern[i][0].print(out);
		pattern[i][1].print(out);
	}
}

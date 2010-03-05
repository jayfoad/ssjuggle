#include <iostream>

#include "MyException.h"
#include "Siteswap.h"
#include "Utility.h"

int main(int argc, char *argv[])
{
	setProgramName(argv[0]);

	if (argc != 2)
		Usage("usage: ssparse <siteswap>");

	try
	{
		SiteswapPattern pattern(argv[1]);
		pattern.print(std::cout);
		std::cout << std::endl;
	}
	catch (MyException& e)
	{
		Fatal(e.what());
	}

	return 0;
}

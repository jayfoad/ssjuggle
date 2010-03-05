#include <cstdlib>
#include <iostream>

#include "Utility.h"

namespace
{
	std::string programName;
}

void setProgramName(std::string name)
{
	size_t slash = name.rfind('/');
	programName =
		std::string(name, slash == std::string::npos ? 0 : slash + 1);
}

void Usage(std::string message)
{
	std::cout << programName << ": " << message << std::endl;
	std::exit(1);
}

void Fatal(std::string message)
{
	std::cerr << programName << ": fatal error: " << message << std::endl;
	std::exit(1);
}

#include <exception>
#include <string>

class MyException : public std::exception
{
	const std::string message;

	virtual const char* what() const throw()
	{
		return message.c_str();
	}

public:
	MyException(const std::string m) : message(m) {}
	virtual ~MyException() throw() {}
};

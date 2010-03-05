#include <exception>
#include <string>

class MyException : public std::exception
{
	const std::string message;

public:
	virtual const char* what() const throw()
	{
		return message.c_str();
	}

	MyException(const std::string m) : message(m) {}
	virtual ~MyException() throw() {}
};

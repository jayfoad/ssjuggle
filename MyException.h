#include <exception>

class MyException : public std::exception
{
	const char *message;

	virtual const char* what() const throw()
	{
		return message;
	}

public:
	MyException(const char *m) : message(m) {}
};

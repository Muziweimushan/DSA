#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include "Object.h"

namespace MyLib
{

#define THROW_EXCEPTION(e, m) throw(e(m, __FILE__, __LINE__))

class Exception : public Object
{
public:
	Exception(const char *msg);
	Exception(const char *file, int line);
	Exception(const char *msg, const char *file, int line);
	
	/*深拷贝*/
	Exception(const Exception &e);
	Exception &operator = (const Exception &e);
	
	const char *message(void) const;
	const char *location(void) const;

	~Exception() = 0;

private:
	char *m_message;
	char *m_location;

	void init(const char *msg, const char *file, int line);
};

/*				计算异常			*/
class ArithmeticException : public Exception
{
public:
	ArithmeticException() : Exception(0) {}
	ArithmeticException(const char *msg) : Exception(msg) {}
	ArithmeticException(const char *file, int line) : Exception(file, line) {}
	ArithmeticException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}
	
	ArithmeticException(const ArithmeticException &e) : Exception(e) {}
	
	ArithmeticException &operator = (const ArithmeticException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};

/*				空指针异常			*/
class NullPointerException : public Exception
{
public:
	NullPointerException() : Exception(0) {}
	NullPointerException(const char *msg) : Exception(msg) {}
	NullPointerException(const char *file, int line) : Exception(file, line) {}
	NullPointerException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}

	NullPointerException(const NullPointerException &e) : Exception(e) {}
	
	NullPointerException &operator = (const NullPointerException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};


/*				下标越界异常			*/
class IndexOutOfBoundException : public Exception
{
public:
	IndexOutOfBoundException() : Exception(0) {}
	IndexOutOfBoundException(const char *msg) : Exception(msg) {}
	IndexOutOfBoundException(const char *file, int line) : Exception(file, line) {}
	IndexOutOfBoundException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}

	IndexOutOfBoundException(const IndexOutOfBoundException &e) : Exception(e) {}
	
	IndexOutOfBoundException &operator = (const IndexOutOfBoundException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};

/*				内存不足异常			*/
class NoEnoughMemoryException : public Exception
{
public:
	NoEnoughMemoryException() : Exception(0) {}
	NoEnoughMemoryException(const char *msg) : Exception(msg) {}
	NoEnoughMemoryException(const char *file, int line) : Exception(file, line) {}
	NoEnoughMemoryException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}

	NoEnoughMemoryException(const NoEnoughMemoryException &e) : Exception(e) {}
	
	NoEnoughMemoryException &operator = (const NoEnoughMemoryException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};

/*				参数非法异常			*/
class InvalidParameterException : public Exception
{
public:
	InvalidParameterException() : Exception(0) {}
	InvalidParameterException(const char *msg) : Exception(msg) {}
	InvalidParameterException(const char *file, int line) : Exception(file, line) {}
	InvalidParameterException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}

	InvalidParameterException(const InvalidParameterException &e) : Exception(e) {}
	
	InvalidParameterException &operator = (const InvalidParameterException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};

/*				非法操作异常			*/
class InvalidOperationException : public Exception
{
public:
	InvalidOperationException() : Exception(0) {}
	InvalidOperationException(const char *msg) : Exception(msg) {}
	InvalidOperationException(const char *file, int line) : Exception(file, line) {}
	InvalidOperationException(const char *msg, const char *file, int line) : Exception(msg, file, line) {}

	InvalidOperationException(const InvalidOperationException &e) : Exception(e) {}
	
	InvalidOperationException &operator = (const InvalidOperationException &e)
	{
		Exception::operator=(e);
		return *this;
	}
};

}




#endif

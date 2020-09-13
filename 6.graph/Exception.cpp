#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "Exception.h"

using namespace MyLib;

void Exception::init(const char *msg, const char *file, int line)
{
	if (NULL != msg)
	{
		this->m_message = strdup(msg);
	}
	else
	{
		this->m_message = NULL;	
	}
	
	if (NULL != file)
	{
		char sl[16] = {0};

		snprintf(sl, sizeof(sl), "%d", line);
		/*location信息格式为文件名:行号*/	
		int len = strlen(file) + 1 + strlen(sl) + 1;
		m_location = static_cast<char *>(malloc(len));

		if (NULL != m_location)
		{
			strcpy(m_location, file);
			strcat(m_location, ":");
			strcat(m_location, sl);
		}
		else
		{
			m_location = NULL;	
		}
	}
	else
	{
		m_location = NULL;	
	}	
}

Exception::Exception(const char *msg)
{
	init(msg, NULL, 0);
}

Exception::Exception(const char *file, int line)
{
	init(NULL, file, line);
}

Exception::Exception(const char *msg, const char *file, int line)
{
	init(msg, file, line);
}

Exception::Exception(const Exception &e)
{
	this->m_message = (NULL == e.m_message) ? NULL : strdup(e.m_message);

	this->m_location = (NULL == e.m_location) ? NULL : strdup(e.m_location);
}

Exception &Exception::operator= (const Exception &e)
{
	/*防止外部的沙雕进行自拷贝*/
	if (this != &e)
	{
		free(this->m_message);
		free(this->m_location);

		this->m_message = (NULL == e.m_message) ? NULL : strdup(e.m_message);

		this->m_location = (NULL == e.m_location) ? NULL : strdup(e.m_location);

	}
	return *this;
}

const char *Exception::message() const
{
	return m_message;
}

const char *Exception::location() const
{
	return m_location;
}

Exception::~Exception()
{
	free(m_message);
	free(m_location);
}



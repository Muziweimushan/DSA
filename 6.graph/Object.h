#ifndef __OBJECT_H__
#define __OBJECT_H__

namespace MyLib
{


class Object
{
public:
	void *operator new (unsigned long int size) throw();
	void operator delete (void *p);

	void *operator new [] (unsigned long int) throw();
	void operator delete [] (void *p);

	bool operator == (const Object &obj);
	bool operator != (const Object &obj);

	virtual ~Object() = 0;
};


}

#endif

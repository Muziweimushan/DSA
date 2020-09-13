#include <cstdlib>

#include "Object.h"

#include <iostream>

using namespace std;
using namespace MyLib;

void *Object::operator new (unsigned long int size) throw()
{
	//cout << "operator new size = " << size << endl;
	return malloc(size);
}

void Object::operator delete (void *p)
{
	//cout << "operator delete" << endl;
	free(p);
}

void *Object::operator new[] (unsigned long int size) throw()
{
	//cout << "operator new[] size = " << size << endl;
	return malloc(size);
}

void Object::operator delete[] (void *p)
{
	//cout << "operator delete[]" << endl;
	free(p);
}

bool Object::operator == (const Object &obj)
{
	/*直接比较地址是否一样的*/
	return (this == &obj);
}

bool Object::operator != (const Object &obj)
{
	return (this != &obj);
}

Object::~Object()
{
	//cout << "~Object()" << endl;
}

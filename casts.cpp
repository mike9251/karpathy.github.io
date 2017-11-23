// casts.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

struct Foo {};
struct Bar {};

class A {};
class B {};
class C :public A, public B {};

class Base { 
public: 
	int a;
	Base() { a = 7; }
private:
};
class Derived : public Base {
public:
	int b;
	Derived() :Base() { b = 8; }
private:
};
class UnrelatedClass {};

struct data {
	char a;
	char b;
};


class Base1 { virtual void vf() {} };

class Derived1 : public Base1 { };

int main()
{
	// const_cast  is typically used to cast away the constness of objects. It is the only C++ style that can do this.
	const int *a = new int(5);
	cout << "a = " << *a << endl;

	int *aa = const_cast<int*>(a);

	cout << "aa = " << *aa << endl;

	*aa = 9;

	cout << "a = " << *a << endl;
	cout << "aa = " << *aa << endl;

	/*Foo* f = new Foo;
	Bar* b1 = (Bar*)f;                              // (1)
	Bar* b2 = static_cast<Bar*>(f);           // (2)
	Bar* b3 = dynamic_cast<Bar*>(f);          // (3)
	Bar* b4 = reinterpret_cast<Bar*>(f);      // (4)
	Bar* b5 = const_cast<Bar*>(f);            // (5)*/

	// reinterpret_cast is intended for low-level casts that yield implementation-dependent and it would not be portable
	//It converts any pointer type to any other pointer type, even of unrelated classes. 
	//The operation result is a simple binary copy of the value from one pointer to the other.
	// Let's consider an example:
	// 
	int value = 261;
	// in binary: 0000 0001 0000 0101
	struct data* pdata = reinterpret_cast<struct data*> (&value);
	cout << "a = " << pdata->a << "b = " << pdata->b << endl;
	//a = 5, b = 1
	//in binary a = 0000 0101, b = 0000 0001


	C d1;
	
	B *b1 = &d1;
	A *a1 = &d1;
	bool bool0 = a1 == &d1;
	bool bool1 = reinterpret_cast<char*>(a1) == reinterpret_cast<char*>(&d1);
	bool bool2 = b1 == &d1;
	bool bool3 = reinterpret_cast<char*>(b1) == reinterpret_cast<char*>(&d1);
	bool bool4 = reinterpret_cast<char*>(a1) == reinterpret_cast<char*>(b1);

	std::cout << bool0 << bool1 << bool2 << bool3 << bool4 << std::endl; // 110
	cout << "c = " << &d1 << "     cast = " << reinterpret_cast<char*>(&d1) << endl;
	cout << "a = " << a1 << "    cast = " << reinterpret_cast<char*>(a1) << endl;
	cout << "b = " << b1 << "    cast = " << reinterpret_cast<char*>(b1) <<endl;

	// static_cast
	//It's valid only if type_name can be converted implicitly to the same type that expression has,
	//or vise versa. Otherwise, the type cast is an error.
	Base base;
	Derived derived;

	// #1: valid upcast
	Base *pBase = static_cast<Base *>(&derived);
	cout << "pBase->a = " << pBase->a << endl;
	//cout << "pBase->b = " << pBase->b << endl;

	// #2: valid downcast
	//Derived *pDerived1 = dynamic_cast<Derived *> (&base);
	Derived *pDerived = static_cast<Derived *> (&base);
	cout << "pDerived->a = " << pDerived->a << endl;
	cout << "pDerived->b = " << pDerived->b << endl;

	// #3: invalid, between unrelated classes
	//UnrelatedClass *pUnrelated= static_cast<UnrelatedClass *> (&derived);

	//dynamic_cast

	
		Base1 b5;
		Derived1 d5;

		Base1 *pb = dynamic_cast<Base1*>(&d5);		// #1
		Derived1 *pd = dynamic_cast<Derived1*>(&b5); 	// #2
		Derived1 *pd2 = dynamic_cast<Derived1*>(&d5);

    return 0;
}


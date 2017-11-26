---
layout: post
comments: true
title:  "Type Casts Notes "
excerpt: "Brief summary of existing type cast methods in C++11: const_cast, static_cast, dynamic_cast and reinterpret_cast."
date:   2017-11-23 20:00:00
---

### const_cast
Typically used to cast away the constness of objects. It is the only C++ style that can do this.
{% highlight c++ %}
const int *a = new int(5);
cout << "a = " << *a << endl;   // return 5
int *b = const_cast<int*>(a);   
cout << "b = " << *b << endl;   // return 5
*b = 9;
cout << "a = " << *a << endl;   // return 9
cout << "b = " << *b << endl;   // return 9

const int c = 10;
const_cast<int&>(c) = 11;
{% endhighlight %}

### reinterpret_cast
Converts any pointer type to any other pointer type, even of unrelated classes. The operation result is a simple binary copy of the value from one pointer to the other.  
Intended for low-level casts that yield implementation-dependent and it would not be portable.  
Let's consider an example:

{% highlight c++ %}
struct Data {
    char a;
    char b;
};

int value = 261;
Data* pdata = reinterpret_cast<Data*> (&value);
cout << "a = " << pdata->a << ", b = " << pdata->b << endl;   //returns a = 5, b = 1
{% endhighlight %}
```
value = 261 = 0000 0000 0000 0000 0000 0001 0000 0101 = 4bytes (int)
a = 5 = 0000 0101 = 1byte (char)
b = 1 = 0000 0001 = 1byte (char)
```
So `reinterpret_cast` just read 1st and 2nd bytes of `value` and wrote it into `a` and `b`. It treats variable `value` of type `int` as a variable of type `struct Data`.

Another way of using `reinterpret_cast` is to compare pointers.
Here is an example:
{% highlight c++ %}
class A {};
class B {};
class C : public A, public B {};

C c;
A *a = &c; // implicit upcasting
B *b = &c; // implicit upcasting

// compare pointers
bool bool0 = a == &c;
bool bool1 = reinterpret_cast<char*>(a) == reinterpret_cast<char*>(&c);
bool bool2 = b == &c;
bool bool3 = reinterpret_cast<char*>(b) == reinterpret_cast<char*>(&c);
bool bool4 = reinterpret_cast<char*>(a) == reinterpret_cast<char*>(b);

std::cout << bool0 << bool1 << bool2 << bool3 << bool4 << std::endl; // 11100
cout << "Adress of c = " << &c << '\n' << "a = " << a << '\n' << "b = " << b << endl;
{% endhighlight %}
`a == &c == true` - `c` is implicitly casted to `A` class, so `a` and casted `c` are identical.  
`reinterpret_cast<char*>(a) == reinterpret_cast<char*>(&c) == true` - treats `a` and `&c` as pointers to `char`, the 1st byte is the same in both pointers.  
`b == &c == true` - `c` is implicitly casted to `B` class, so `b` and casted `c` are identical. 
`reinterpret_cast<char*>(b) == reinterpret_cast<char*>(&c) == false` - after upcasting `b` kind of points at part of `c` which has offset of `sizeof(A)`.  
`reinterpret_cast<char*>(a) == reinterpret_cast<char*>(b) == false` - after upcasting `a` points at the beginning of the `c` and `b` at part of `c` which has offset of `sizeof(A)`.  
Output of the last string is:  
Adress of c = 000000E77974F664  
a = 000000E77974F664  
b = 000000E77974F665  

### static_cast
Valid only if type_name can be converted implicitly to the same type that expression has, or vise versa. Otherwise, the type cast is an error. It can be used to force implicit conversions such as `non-const` object to `const`, `int` to `double`. It can be also be used to perform the reverse of many conversions such as `void*` pointers to typed pointers, base pointers to derived pointers. But it cannot cast from `const` to `non-const` object.

{% highlight c++ %}
class Base { 
public: 
	int a;
	Base() { a = 7; }
};

class Derived : public Base {
public:
	int b;
	Derived() :Base() { b = 8; }
};

class UnrelatedClass {};

int main(){
    Base base;
    Derived derived;

    // valid upcast
    Base *pBase = static_cast<Base *>(&derived);
    cout << "pBase->a = " << pBase->a << endl;
    //cout << "pBase->b = " << pBase->b << endl;

    // valid downcast
    Derived *pDerived = static_cast<Derived *> (&base);
    cout << "pDerived->a = " << pDerived->a << endl; // output 7
    cout << "pDerived->b = " << pDerived->b << endl; // output some garbage, Base doesn't contain b

    // invalid, between unrelated classes
    //UnrelatedClass *pUnrelated= static_cast<UnrelatedClass *> (&derived);
    return 0;
}
{% endhighlight %}
### dynamic_cast
This kind of cast is used to perform safe downcasting, i.e., to determine whether an object is of a particular type in an inheritance hierarchy. It is the only cast that may have a significant runtime cost.
`dynamic_cast` involves a run-time type check. If the object bound to the pointer is not an object of the target type, it fails and the value is 0. If it's a reference type when it fails, then an exception of type `bad_cast` is thrown. So, if we want dynamic_cast to throw an exception (bad_cast) instead of returning 0, cast to a reference instead of to a pointer. Note also that the dynamic_cast is the only cast that relies on run-time checking.

{% highlight c++ %}
class Base { 
    virtual void vf(){}
};

class Derived : public Base { };

int main() 
{
    Base *pbd = new Derived;
    Base *pb = new Base;
    Derived *pd;

    pd = dynamic_cast<Derived*>(pbd);	// correct, pbd points at an object of type Derived
    pd = dynamic_cast<Derived*>(pb);	// invalid, return nullptr
    
    Base b;
    Derived d;
    Base *pb = dynamic_cast<Base*>(&d);		// correct upcasting
    Derived *pd = dynamic_cast<Derived*>(&b);  // invalid downcasting, return nullptr

    return 0;
}
{% endhighlight %}

A use case example:
{% highlight c++ %}
#include <iostream>
#include <string>
using namespace std;

class Window
{
public:
	Window(){}
	Window(const string s):name(s) {};
	virtual ~Window() {};
	void getName() { cout << name << endl;};
private:
	string name;
};

class ScrollWindow : public Window
{
public:
	ScrollWindow(string s) : Window(s) {};
	~ScrollWindow() {};
	void scroll() { cout << "scroll()" << endl;};
};

void DoSomething(Window *w)
{
	w->getName();
	// w->scroll();  // class "Window" has no member scroll

	// check if the pointer is pointing to a scroll window
	ScrollWindow *sw = dynamic_cast<ScrollWindow*>(w);

	// if not null, it's a scroll window object
	if(sw) sw->scroll();
}

int main()
{
	Window *w = new Window("plain window");
	ScrollWindow *sw = new ScrollWindow("scroll window");

	DoSomething(w);
	DoSomething(sw);

	return 0;
}
{% endhighlight %}
`DoSomething(Window* w)` is passed down `Window` pointer. It calls `scroll()` method which is only available from `Scroll` object. So, in this case, we need to check if the object is the `Scroll` type or not before the call to the `scroll()` method.

Example (static_cast/dynamic_cast):
{% highlight c++ %}
class Dog {
public:
    virtual ~Dog() {};
};

class YellowDog : public Dog {
public:
    int a;
    void bark() { std::cout << "Woof\n"; }
};

Dog *dog = new Dog();

YellowDog *sydog = dynamic_cast<YellowDog*>(dog);
YellowDog *dydog = static_cast<YellowDog*>(dog);
sydog->bark();
dydog->bark();
{% endhighlight %}

In both cases the result is `Woof`, because bark() is treated as a static function.
Change `bark()` to `void bark() { std::cout << "Woof\n" << a; }`then `bark()` won't be static anymore and `sydog->bark()` prints `Woof + some garbage` because `Dog` doesn't have the field `int a` and `dydog->bark()` will trow an exception because of the access through `nullptr`.
It's better not to use `static_cast` here and instead use `dynamic_cast` but first check that upcasting performed correctly:
{% highlight c++ %}
YellowDog *dydog = static_cast<YellowDog*>(dog);
if(dydog)
    dydog->bark();
{% endhighlight %}

A better way is to use dynamic polymorphism:
{% highlight c++ %}
class Dog {
public:
    virtual void bark() {}
    virtual ~Dog() {}
};

Dog *dog = new YellowDog();
dog->bark();
{% endhighlight %}

[More info of type casts](http://www.bogotobogo.com/cplusplus/typecast.php)

---
layout: post
comments: true
title:  "C++ Notes "
excerpt: "Brief overview of modern C++ features"
date:   2019-11-08 20:00:00
---

### lvalue and rvalue references
lvalue - anything that can be placed on the left side of an expression
rvalue - literals (e.g. 5), temporary values (e.g. x+1), and anonymous objects (e.g. Fraction(5, 2)).

Below we can see an example of the lvalue and rvalue references usage:
{% highlight c++ %}
void printInt(int &a) { std::cout << "printInt with an lvalue reference" << std::endl; }
void printInt(int &&a) { std::cout << "printInt with an rvalue reference" << std::endl; }

int main()
{
    int a = 5; // a - lvalue
    int &b = a; // b - lvalue reference
    int &&c = 5; // c - rvalue reference initialized with an rvalue 5
    
    printInt(a);// call printInt(int &a)
    printInt(10);// call printInt(int &&a)
    printInt(std::move(a));// call printInt(int &&a)
    
    return 0;
}
{% endhighlight %}

Rvalue reference usage:
1. Move semantic
2. Perfect forwarding


### Move semantic
When some complex objects are passed by value the copy constructor gets invoked to perform deep copy (doesn't happen when it is passed by reference). If the object is an `rvalue` then we will first create this rvalue object and then perform `copy constructor`, then call destructor for the rvalue object. Instead of those unnecessary steps we can just move the rvalue object with `move constructor`.
{% highlight c++ %}
class myVector
{
public:
    myVector(int size)
    {
    	std::cout << "constructor!\n";
    	size_ = size;
    	arr_ = new int[size];
        for (int i = 0; i < size; i++)
        {
 	    arr_[i] = i;
        }
    }

    myVector(const myVector& rhs)
    {
	std::cout << "Copy constructor!\n";
	size_ = rhs.size_;
	arr_ = new int[size_];
	for (int i = 0; i < size_; i++)
	{
	    arr_[i] = rhs.arr_[i];
	}
    }

    myVector(myVector&& rhs)
    {
	std::cout << "Move constructor!\n";
	size_ = rhs.size_;
	arr_ = rhs.arr_;
	rhs.size_ = 0;
	rhs.arr_ = nullptr;
    }
    
    ~myVector()
    {
	delete[] arr_;
    }

    int size_;
    int *arr_;
};

myVector createMyVector()
{
    myVector vec(10);
    return vec;
}

// recieves myVector object by value (copy constructor gets invoked when lvalue object passed
// and move constructor gets invoked when rvalue object passed)
void printMyVector(myVector vec)
{
    for (int i = 0; i < vec.size_; i++)
    {
    	std::cout << vec.arr_[i] << " ";
    }
}

int main()
{
    // use move constructor
    printMyVector( createMyVector() ); // createMyVector() - returns an rvalue, so no copy performed

    //use copy construvtor
    myVector myVec = myVector(10);
    printMyVector(myVec); // myVec is an lvalue object, copy constructor is invoked to create a copy of myVec
    
    // use move constructor
    printMyVector(std::move(myVec)); // std::move created an rvalue from an lvalue object.
    //After move constructor call the myVec object will be empty
    return 0;
}

{% endhighlight %}
Move semantic is implemented for all STL containers.

### Perfect forwarding
We would like to perform argument forwarding without costly and unnecessary copy constructor and keeping rvalue as rvalue / lvalue as lvalue.

### Reference collapsing rules:
1. T& & ==> T&
2. T& && ==> T&
3. T&& & ==> T&
4. T&& && ==> T&&

{% highlight c++ %}
template <typename T>
void relay(T&& arg)
{
    printMyVector(std::forward<T>(arg));
}
	
int main()
{
    //use move construvtor
    relay(myVector(10)); // pass to the relay an rvalue, printMyVector will be called with an rvalue
    // T&& arg is initialized with an rvalue reference, T == myVector&& ==> T&& == myVector&& && ==> myVector&&
    
    //use copy constructor
    myVector myVec = myVector(10);
    relay(myVec); // pass to the relay an lvalue
    // T&& arg is initialized with an lvalue reference, T == myVector& ==> T&& == myVector& && ==> myVector&
    return 0;
}

{% endhighlight %}

So in the case above, when relay takes on an rvalue, after reference collapsing we get rvalue reference, otherwise an lvalue reference.

T&& is a universal reference if:
1. T is a template type.
2. Reference collapsing happens to T.

### User defined literals
If we want to associate values with some metrics (m, cm, mm, ...) we can define corresponding literals.

{% highlight c++ %}
long double operator"" _cm(long double x) { return 10 * x; }
long double operator"" _m(long double x) { return 100 * x; }
long double operator"" _mm(long double x) { return x; }
int main()
{       //next values will be recalculated in run time
	long double h = 5.2_cm;
	long double h1 = 5.2_m;
	long double h2 = 502.0_mm;
	cout << h << " mm " << h1 << " mm " << h2 << " mm\n";
	return 0;
}
{% endhighlight %}

### Compiler Generated Functions

### C++ 03
1. Default constructor      (generated only if no constuctor is defined by user)
2. Copy constructor         (generated only if no 5, 6 defined)
3. Copy assignment operator (generated only if no 5, 6 defined)
4. Destructor
### C++11
5. Move constructor         (generated only if no 2, 3, 4, 6 defined)
6. Move assignment operator (generated only if no 2, 3, 4, 5 defined)

So if we declare an empty class in fact we get a class with 6 compiler generated functions:
{% highlight c++ %}
class EmptyClass
{}

or 

class EmptyClass
{
    //C++03
    EmptyClass();
    EmptyClass(const EmptyClass &rhs);
    EmptyClass& operator=(const EmptyClass &rhs);
    ~EmptyClass();
    //C++11
    EmptyClass(EmptyClass &&rhs);
    EmptyClass& operator=(EmptyClass &&rhs);
};
{% endhighlight %}
### shared_ptr and multithreading
Reference counter is atomic mechanism so in multithreaded environment the number of shared pointers holding an object will be correctly calculated. Thread safe operations on the object will work ok (e.g. reading a value). BUT if any object modifying operation is used we need to make them thread safe with mutex.

Consider an example when we have a global shared_ptr and two functions:
{% highlight c++ %}
shared_ptr<Widget> globalSharedPtr(new Widget);

void read() {
    shared_ptr<Widget> x = globalSharedPtr;
    // do something with x
}

void write() {
     globalSharedPtr.reset( new Widget );
}
{% endhighlight %}

Approximate impl of shared_ptr:
{% highlight c++ %}
shared_ptr::shared_ptr(const shared_ptr<T>& x) {
A1:    pointer = x.pointer;
A2:    counter = x.counter;
A3:    atomic_increment( *counter );
}

shared_ptr<T>::reset(T* newObject) {
B1:    if( atomic_decrement( *counter ) == 0 ) {
B2:        delete pointer;
B3:        delete counter;
B4:    }
B5:    pointer = newObject;
B6:    counter = new Counter;
}
{% endhighlight %}
If one thread starts copying globalSharedPtr (read) and another reseting (write), then it's possible to get race conditions over pointer/counter variables.

{% highlight c++ %}
shared_ptr<Widget> globalSharedPtr(new Widget);
mutex_t globalSharedPtrMutex;

void resetGlobal(Widget* x) {
    write_lock_t l(globalSharedPtrMutex);
    globalSharedPtr.reset( x );
}

shared_ptr<Widget> getGlobal() {
    read_lock_t l(globalSharedPtrMutex);
    return globalSharedPtr;
}

void read() {
    shared_ptr<Widget> x = getGlobal();
    // do something with x
}

void write() {
     resetGlobal( new Widget );
}
{% endhighlight %}

### weak_ptr
Smart pointer implementation, which allows sharing a raw pointer among other shared_ptr's instances and it doesn't increment reference
counter.
{% highlight c++ %}
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::weak_ptr<MyClass> wptr1(ptr1); // number of references doesn't change
{% endhighlight %}
To get acces to the object the `weak_ptr` should be transformed into `shared_ptr`:
{% highlight c++ %}
std::shared_ptr<MyClass> ptr2 = wptr1.lock() or auto ptr2 = wptr1.lock()
{% endhighlight %}


Fix `Circular dependency issues`:
{% highlight c++ %}
class Human
{
public:
	Human(std::string const & name) {
		m_name = name;
		std::cout << m_name.c_str() << " is created!\n";
	}
	~Human()
	{
		std::cout << m_name.c_str() << " is destroyed!\n";
	}
	std::string m_name;
	std::weak_ptr<Human> p_fiend;
};
std::shared_ptr<Human> tom(new Human("Tom"));
std::shared_ptr<Human> casey(new Human("Casey"));

tom->p_fiend = casey;
casey->p_fiend = tom;
{% endhighlight %}
{% highlight c++ %}
Output:
Tom is created!
Casey is created!
Casey is destroyed!
Tom is destroyed!
{% endhighlight %}

### unique_ptr
Smart pointer implementation, which solely holds a raw pointer of an object! It implements `move semantic` - 
{% highlight c++ %}
std::unique_ptr<int> uptr1(new int(13));
std::cout << "uptr1 = " << *uptr1 << std::endl;
{% endhighlight %}
In order to pass a pointer from one unique_ptr to another we need to use `std::move`:
{% highlight c++ %}
std::unique_ptr<Human> u_pp(new Human("Tom"));
std::unique_ptr<Human> u_pp2;
std::cout << ((static_cast<bool>(u_pp)) ? "\nu_pp is not NULL" : "\nu_pp is NULL\n");
std::cout << (static_cast<bool>(u_pp2) ? "\nu_pp2 is not NULL" : "\nu_pp2 is NULL\n");

u_pp2 = std::move(u_pp);

std::cout << (static_cast<bool>(u_pp) ? "\nu_pp is not NULL" : "\nu_pp is NULL\n");
std::cout << (static_cast<bool>(u_pp2) ? "\nu_pp2 is not NULL" : "\nu_pp2 is NULL\n");
{% endhighlight %}
{% highlight c++ %}
Output:
Tom is created!
u_pp is not NULL
u_pp2 is NULL

u_pp is NULL
u_pp2 is not NULL
Tom is destroyed!
{% endhighlight %}
We can get a shared pointer from a unique one:
{% highlight c++ %}
std::shared_ptr<int> shared = std::move(uptr1);
{% endhighlight %}
After that `uptr1` will be empty and posession of the objects address went to `shared`.
We can't get a `weak_ptr` from `unique_ptr` (because `weak_ptr` doesn't inform that it holds a pointer)!

### auto_ptr - is not recomended to use
First attempt to standartize smart pointers released in C++98. It implements `move semantic` through copy constructor and redefined operator=. As a result it's very error prone. Example:
{% highlight c++ %}
void some_func(std::auto_ptr<int> auto_pp)
{
}
std::auto_ptr<int> auto_p(new int(10));
some_func(auto_p);
std::cout << *auto_p; //error
{% endhighlight %}
Trying to dereference the auto_p will cause program crash because when `some_func(auto_p) ` is called the copy constructor of `std::auto_ptr` will move pointer that auto_p holds to the `some_func` parameter - auto_p is empty. Fix it with:
{% highlight c++ %}
void some_func(std::auto_ptr<int> &auto_pp)
{
}
Also `std::auto_ptr` incorrectly works with arrays - to free memory it always calls `delete` which will cause memory leaks when working with arrays.
{% endhighlight %}
One possible use case:
{% highlight c++ %}
int main(){
    MyClass* obj = new MyChild();
    std::shared_ptr<MyClass> p(obj);
    return 0;
}
{% endhighlight %}
Both `MyChild` and `MyClass` object will be deleted (even without virtual destructors).

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
# C++ 03  
1.Default constructor      (generated only if no constuctor is defined by user)  
2.Copy constructor         (generated only if no 5, 6 defined)  
3.Copy assignment operator (generated only if no 5, 6 defined)  
4.Destructor  
# C++11  
5.Move constructor         (generated only if no 2, 3, 4, 6 defined)  
6.Move assignment operator (generated only if no 2, 3, 4, 5 defined)  

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

<div class="imgcap">
<img src="/assets/compiler_generated_functions.png">
</div>

### Disallow Compiler Generated Functions
In C++11 we can dissalow Compiler Generated Functions with `delete` modifier:
{% highlight c++ %}
class SomeClass
{
public:
    //C++03
    SomeClass(int a = 0) {m_a = a;};                   // define default constructor
    SomeClass(const SomeClass &rhs) = delete;          // forbid compiler to generate copy constructor
private:
    int m_a;
};
{% endhighlight %}
In C++03 just declare unwanted functions into private section without definition:
{% highlight c++ %}
class SomeClass
{
public:
    //C++03
    SomeClass(int a = 0) {m_a = a;};           // define default constructor
private:
    SomeClass(const SomeClass &rhs);          // forbid compiler to generate copy constructor
};
{% endhighlight %}

If we define destructor in private section, in order to destruct the object we need a helper function to do so. In this case we need to create the class object on heap. Creating an object on stack evantually will try to call the destructor - so this case won't compile. 
### Const

`const` before `*` - pointer to a constant data, can't dereference the pointer and change it's value but can change the pointer itself.
`const` after `*` - constant pointer, can dereference the pointer and change the value it points to but can't change the address it points to.
{% highlight c++ %}
const int a = 10;
const int *p  = &a;
*p = 7; // doesn't compile
p++; // ok

int b = 11;
int * const q = &b;
*q = 12; //b = 12
{% endhighlight %}

Hacky way to change a constant value:
{% highlight c++ %}
const int a = 10;
const_cast<int&>(a) = 11; // ok
{% endhighlight %}

{% highlight c++ %}
const int * const func(const int * const &arg);
//1. return value is a const pointer to a const data
//2. the argument is a reference to a const pointer to a const data
{% endhighlight %}

## Const function
A member function which doesn't modify any member variable, can call only const functions, otherwise compiler error.
{% highlight c++ %}
void func() const;
{% endhighlight %}

Const modifier can be used to overload functions:
{% highlight c++ %}
class Test
{
public:
    void func() const; //(1)
    void func();       //(2)
};

int main()
{
    Test obj;
    obj.func(); // (1)
    
    const Test obj2;
    obj2.func(); // (2)
    
    return 0;
}
{% endhighlight %}

When used as argument type modifier and arguments are passed by value can't be used to overload functions - the case below won't compile:
{% highlight c++ %}
void func(int a);
void func(const int a);
{% endhighlight %}
When used as argument type modifier and arguments are passed by reference can be used to overload functions:
{% highlight c++ %}
void func(int &a);         //(1) - receives lvalue reference
void func(const int &a);   //(2) - receives rvalue reference

int main()
{
    int a = 10;
    func(a);               //(1)
    func(5);               //(2)
    func(std::move(a));    //(2)
    return 0;
}
{% endhighlight %}

## Virtual destructor
{% highlight c++ %}
class Cat
{
public:
    ~Cat() { std::cout << "Cat is destroyed\n"; }
};

class WhiteCat : public Cat
{
public:
    ~WhiteCat() { std::cout << "WhiteCat is destroyed\n"; }
};

int main()
{
    Cat *cat = new WhiteCat;
    delete cat; // prints Cat is destroyed
    return 0;
}
{% endhighlight %}
Only part of the WhiteCat object is destroyed (base part). To fix this issue we need to declare the destructor of a base class as `virtual`.
{% highlight c++ %}
class Cat
{
public:
    virtual ~Cat() { std::cout << "Cat is destroyed\n"; }
};

class WhiteCat : public Cat
{
public:
    ~WhiteCat() { std::cout << "WhiteCat is destroyed\n"; }
};

int main()
{
    Cat *cat = new WhiteCat;
    delete cat; // prints WhiteCat is destroyed
                          Cat is destroyed
    return 0;
}
{% endhighlight %}
When at least one function in the base class is vurtual the `virtual functions table, vftable` is created for the class. Each object of such class will hold a pointer to this table - `__vfptr` (only a pointer not the copy of the table). So two objects will hold `__vfptr`s to the same table. When we derive a child class from a base class with virtual functions the `vftable` is copied from the base class and each object of the derived class will hold the `__vfptr` to this new table. This table will hold pointers to the functions in the base class. So, if we don't override virtual functions of the base class inside the child class, calling them from a child class will first invoke looking at the `vftable` of the child class and call the corresponding function (which is base class function). If we override vurtual functions in child class then we replace vurtual functions (their addresses) in the `vftable` of the child class with the addresses of overriding functions in child class. This mechanism allows `dynamic polymorphism`.  
So in the example above, when `delete cat` is called, as the cat hold a pointer to the WhiteCat object (which has a pointer to its own vftable), we first need to decide which destrucctor to invoke - look at the WhiteCat's vftable and find the destructor address (which is the WhiteCat's destructor). After that destructor of the Cat is also invoked.
{% highlight c++ %}
class Cat
{
public:
    virtual ~Cat() { std::cout << "Cat is destroyed\n"; }
    virtual void func() { std::cout << "Some function\n"; }
};

class WhiteCat : public Cat
{
public:
    ~WhiteCat() { std::cout << "WhiteCat is destroyed\n"; }
};

int main()
{
    Cat *cat1 = new Cat;
    Cat *cat3 = new Cat;
    Cat *cat2 = new WhiteCat;
    delete cat1;
    delete cat2;
    delete cat3;
    return 0;
}
{% endhighlight %}
The `cat1` and `cat2` objects hold `vfptr`s to the same `vftable` - address is `0x00007ff7155cb858`.
Inside this table there are addresses of two functions - destructor `0x00007ff71555d48d` and func() `0x00007ff71555ec11`.
The `cat3` object holds a `vfptr` to a different `vftable` - address is `0x00007ff7155cb8a8`.
Inside this table there are addresses of two functions - destructor `0x00007ff71555f030` and func() `0x00007ff71555ec11`. Destructor address is different from the `cat1/2`s objects as it is overriden with the `WhiteCat`s destructor. The address of the func() function is the same because it's left untouched in the `WhiteCat` class.
<div class="imgcap">
<img src="/assets/virt_tables.PNG">
</div>

Another solution to correct object destruction is to use `shared_ptr`s:
{% highlight c++ %}
class Cat
{
public:
    ~Cat() { std::cout << "Cat is destroyed\n"; }
};

class WhiteCat : public Cat
{
public:
    ~WhiteCat() { std::cout << "WhiteCat is destroyed\n"; }
};

int main()
{
    shared_ptr<Cat> cat = make_shared<WhiteCat>();
    return 0;  // prints WhiteCat is destroyed
                         Cat is destroyed
}
{% endhighlight %}

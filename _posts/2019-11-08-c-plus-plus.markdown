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
1. Default constructor      (generated only if no constuctor is defined by user)
2. Copy constructor         (generated only if no 5, 6 defined)
3. Copy assignment operator (generated only if no 5, 6 defined)
4. Destructor  
# C++11
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

<div class="imgcap">
<img src="/assets/compiler_generated_functions.png">
</div>

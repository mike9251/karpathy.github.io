---
layout: post
comments: true
title:  "Smart Pointers Notes "
excerpt: "Short overview of Smart Pointers in C++11: shared_ptr, weak_ptr and unique_ptr."
date:   2017-09-14 20:00:00
---

### shared_ptr
Smart pointer implementation, which allows sharing a raw pointer among other shared_ptr's instances.
When a shared pointer (sPtr) is created the block of memory (T) is allocated and a control block (ctrlBlock) is also created, which holds `use_count` and `weak_count` parameters. The sPtr contains two pointers: to the T and to the ctrlBlock. When `use_count` == 0 the T is freed as no shared pointers own it. When `weak_count` == 0 the ctrlBlock is destroyed as there is no point in keeping it alive. `use_count = # of shared pointers` and `weak_count = # of weak pointers + 1 if use_count > 0`.In case when `use_count` == 0 and `weak_count` > 0 the memory block T is freed but there is still at least one weak pointer to the T, so in order to correctly handle the dangling pointer access we keep the ctrlBlock alive. 
{% highlight c++ %}
std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>(); // fastest and exception safe method
{% endhighlight %}
or
{% highlight c++ %}
std::shared_ptr<MyClass> ptr1(new MyClass()); // not exception safe method
{% endhighlight %}
or
{% highlight c++ %}
std::shared_ptr<MyClass> ptr1;
ptr1 = std::make_shared<MyClass>();
{% endhighlight %}
or 
{% highlight c++ %}
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
{% endhighlight %}

But don't do that:
{% highlight c++ %}
MyClass obj;
std::shared_ptr<MyClass> ptr1(&obj);
{% endhighlight %}
because in this case the object `obj` is allocated in stack and when `ptr1` pointer will be deleting this object the program will crush.
The smart pointer expects that the object which pointer it holds is allocated in the heap.

If you want to share raw pointer, the safe method is:
{% highlight c++ %}
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::shared_ptr<MyClass> ptr2(ptr1);
{% endhighlight %}
In this case two smart pointers will be holding address of the object (ref count = 2). If one of the shared pointers exits it's local
space it will be destroyied, reference counter will be decresed by 1, but the object `obj` will exist and the other shared pointer will
be holding it's address.

Not safe method is to let several shared pointers hold object's adress through the raw pointer `obj`: 
{% highlight c++ %}
{
    MyClass *obj = new MyClass();
    std::shared_ptr<MyClass> ptr1(obj);
    {
        std::shared_ptr<MyClass> ptr2(obj);
    } // (1)
    ptr1->do_something(); // (2)
}// (3)
{% endhighlight %}
In this case reference counters will be unique for each shared pointers and if one of the shared pointers leaves it's locas space (1)
it will trigger dectructor for `obj` object. But the other pointer still thinks that it's holding adress of some object and any attempt
to do something with it (2) or leaving it's local space (3) will cause an exception.  

Shared pointers created with the `make_shared` call use default deleter - the class destructor, to destroy allocated object when the ref counter is 0. However, we can pass a custom deleter to the shared pointer constructor:
{% highlight c++ %}
shared_ptr<MyClass> ptr_custom_deleter = shared_ptr<MyClass>(new MyClass(), [](MyClass* ptr) { delete ptr; cout << "Object is custom deleted\n"; });
{% endhighlight %}
This may be helpfull if we allocate an array of of objects and pass it to the shared pointer. Before C++17 shared pointers that own a pointer to an array by default will call `delete` not `delete[]`.
{% highlight c++ %}
shared_ptr<MyClass> p_arr = shared_ptr<MyClass>(new MyClass[5], [](MyClass* ptr) {delete[] ptr; }); // free memory for 5 elements
shared_ptr<Dog[]> p_arr(new Dog[5]); // free memory for 5 elements, delete[] will be called. C++17 feature.
shared_ptr<MyClass> p_arr2(new MyClass[5]); // when ref counter == 0 only first object will be deleted and the rest 4 objects will be memleaked - it will cause an exception
{% endhighlight %}
In some cases use of shared_ptr can cause `Circular dependency issues`. Consider an example:
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
	std::shared_ptr<Human> p_fiend;
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
{% endhighlight %}

In this case objects `tom` and `casey` will be created but they won't be destroyed, because reference counters will be = 2 and after leaving local space ref counters will not be set to 0. To avoid this issue we need to use `weak_ptr`.

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
Weak pointers complement Shared pointers.
{% highlight c++ %}
MyClass *obj = new MyClass();
std::shared_ptr<MyClass> ptr1(obj);
std::weak_ptr<MyClass> wptr1(ptr1); // number of references doesn't change
{% endhighlight %}
To get acces to the object the `weak_ptr` should be transformed into `shared_ptr`:
{% highlight c++ %}
std::shared_ptr<MyClass> ptr2 = wptr1.lock() or auto ptr2 = wptr1.lock()
{% endhighlight %}


Main use is to fix `Circular dependency issues`:
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
	
In contrast to the `shared_ptr` unique pointers correctly work by default with pointers to arrays starting from C++11:
{% highlight c++ %}
unique_ptr<MyClass[]> p_arr(new MyClass[3]); // free memory for 5 elements, delete[] will be called. C++11 feature.
{% endhighlight %}

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

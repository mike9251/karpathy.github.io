---
layout: post
comments: true
title:  "C++ Concurrency"
excerpt: "Brief overview of concurrency in C++"
date:   2019-11-25 20:00:00
---

### Process vs Thread
Process owns some memory and contains at least one working thread. Processes communicate with each other through `Interproces Communication` channels such as: file, pipe, message queue, ...  
Threads live inside a process and communicate with each other through some shared memory.
<div class="imgcap">
<img src="/assets/c-plus-plus-concurrency/multiprocessing.JPG">
</div>
<div class="imgcap">
<img src="/assets/c-plus-plus-concurrency/multithreading.JPG">
</div>

Threads start with executing a function they were passed during creation. Next we can `join()` new thread with main thread (main thread waits untill new thread finishes its function) or `detach()` new thread (main thread doesn't wait for child threads completion and continues its execution). If thread is detached then it bacomes a `deamon` thread and will be managed by C++ runtime (it can't be joined anymore).  
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void func_1()
{
	cout << "func_1" << endl;
}

int main()
{
	thread t1(func_1);
	t1.join();

	if (t1.joinable())
		t1.join();
	return 0;
}
{% endhighlight %}

We can pass parameters into a function. Parameters are passed by value:
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void func_1(string msg)
{
	cout << "func_1: " << msg << endl; // Some message
}

int main()
{
	string s = "Some message";

	thread t1(func_1, s);
	t1.join();

	cout << "main: " << s << endl; // Some message

	if (t1.joinable())
		t1.join();
	return 0;
}
{% endhighlight %}
Parameters passed by reference:
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

using namespace std;

void func_1(string &msg)
{
	cout << "func_1: " << msg << endl; // Some message
  msg = "New message";
}

int main()
{
	string s = "Some message";

	thread t1(func_1, std::ref(s));
	t1.join();

	cout << "main: " << s << endl; // New message

	if (t1.joinable())
		t1.join();
	return 0;
}
{% endhighlight %}

We also can pass functors into threads:
{% endhighlight %}
Parameters passed by reference:
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

using namespace std;

class Functor
{
public:
	void operator()(string &msg)
	{
		cout << "Functor Thread ID: " << this_thread::get_id() << endl;   // Functor Thread ID: 9228
		cout << "Functor: " << msg << endl;
		msg = "New message";
	};
};

int main()
{
	cout << "Thread capacity: " << thread::hardware_concurrency() << endl;
	cout << "Main Thread ID: " << this_thread::get_id() << endl;
  
	string s = "Some message";
	Functor fnctr;
	thread t1(fnctr, std::ref(s));
  cout << "t1 id: " << t1.get_id() << endl;    // t1 id: 9228. thread id is the same as in Functor() 
	t1.join();
  
  cout << "main: " << s << endl;

	if (t1.joinable())
		t1.join();
	return 0;
}
{% endhighlight %}
`std::thread::hardware_concurrency()` - returns the number of threads supported on the hardware. Having too many threads (`oversubscription`) can harm performance because of context switching.  
`std::this_thread::get_id()` - returns id of the current thread.  
`t1.get_id()` - return id of the t1 thread.  
After `t1.join()` t1 thread is finished and its id = 0.  
Also we can pass parameters into a thread as rvalue reference, using move semantic (std::move) to reduce unnecessary copies.
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

using namespace std;

class Functor
{
public:
	void operator()(string &&msg)
	{
		cout << "Functor Thread ID: " << this_thread::get_id() << endl;
		cout << "Functor: " << msg << endl;    // Some message
		msg = "New message";
	};
};

int main()
{
	cout << "Thread capacity: " << thread::hardware_concurrency() << endl;
	cout << "Main Thread ID: " << this_thread::get_id() << endl;
  
	string s = "Some message";
	Functor fnctr;
	thread t1(fnctr, std::move(s));
  cout << "t1 id: " << t1.get_id() << endl;    // t1 id: 9228. thread id is the same as in Functor() 
	t1.join();
  
  cout << "main: " << s << endl;   // nothing as s is empty

	if (t1.joinable())
		t1.join();
	return 0;
}
{% endhighlight %}

Threads can't be copied but can be modev with `std::move`.

### Race condition and Mutex
Exmple with race condition over std::cout:
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>

void thread_func(string &&msg, int t_id)
{
	for (int i = 0; i < 1000; i++)
	{
			std::cout << "thread " << t_id << msg << endl;
	}
}

int main()
{
	std::thread t1(thread_func, std::string("Hello world!"), 1);
	std::thread t2(thread_func, std::string("Another message!"), 2);

	t1.join();
	t2.join();
	return 0;
}
{% endhighlight %}
Output is messy because two threads write at the cout simultaneously. To fix this we can use mutex to syncrhonize threads. So we need to "bind" cout with mutex.
{% highlight c++ %}
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

void shared_cout(string &&msg, int t_id)
{
	mu.lock();
	cout << "thread " << t_id << " msg: " << msg << endl;
	mu.unlock();
}

void thread_func(string &&msg, int t_id)
{
	for (int i = 0; i < 1000; i++)
	{
		try
		{
			shared_cout(std::move(msg), t_id);
		}
		catch (exception &ex)
		{
			cout << "Exception is caught: " << ex.what() << endl;
		}
	}

}

int main()
{
	std::thread t1(thread_func, std::string("Hello world!"), 1);
	std::thread t2(thread_func, std::string("Another message!"), 2);

	t1.join();
	t2.join();
	return 0;
}
{% endhighlight %}
This will work but it is still not safe - if an exception occures during cout (after the mutex was locked) the mutex will remain locked and no one will be able to use shared_cout function. We can use RAII semantic - create a mutex wrapper on stack which will take and lock a mutex when created and unlock when we go out of its scope (`std::lock_guard`).
{% highlight c++ %}
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

void shared_cout(string &&msg, int t_id)
{
	std::lock_guard<std::mutex> locker(mu);
	cout << "thread " << t_id << " msg: " << msg << endl;
}

void thread_func(string &&msg, int t_id)
{
	for (int i = 0; i < 1000; i++)
	{
		try
		{
			shared_cout(std::move(msg), t_id);
		}
		catch (exception &ex)
		{
			cout << "Exception is caught: " << ex.what() << endl;
		}
	}

}

int main()
{
	std::thread t1(thread_func, std::string("Hello world!"), 1);
	std::thread t2(thread_func, std::string("Another message!"), 2);

	t1.join();
	t2.join();
	return 0;
}
{% endhighlight %}
Using this synchronization method makes std::cout thread safe when using through `shared_cout()`.  

Avoid data race:
1. Use mutex to sinchronize data access
2. Do not leak a reference or pointer as it allows to modify the shared data without protection
3. Design thread-safe interfaces (for example, stack has .pop() and .top() methods, suppose two threads look at the top of the stack with .top() method and decide to remove it, so the first thread calls .pop() and the second thread will pop different element)

### Deadlock
When we use more than one mutex to protect shared data `deadlock` can happen. For example, one thread locks mu1 and mu2, another thread locks mu2 and mu1. Then it is possible that mu1 will be locked by thread 1 and mu2 by thread 2. Then both threads will wait to lock mu1 and mu2.
To avoid deadlock:
1. Try to use one mutex
2. Use std::lock(mu1, mu2, ...) function to safely lock more than one mutex (it has some anti deadlock mechanizm) 
3. Lock mutexes in the same order (mu1 -> mu2)
4. Avoid locking a mutex and then calling some user defined function (we don't know but it may lock some other mutexes)
{% highlight c++ %}
std::mutex mu1;
std::mutex mu2;

std::lock(mu1, mu2);

// We need to say the lock_guard not to lock mutexes again but just take ownership of mutexes
std::lock_guard<std::mutex> locker1(mu1, std::adopt_lock);
std::lock_guard<std::mutex> locker2(mu2, std::adopt_lock);
{% endhighlight %}

### Unique_lock
Gives more flexebilities than `lock_guard`. With `unique_lock` we can lock, unlock mutex, deffer locking. It also can be moved (with `std::move`) in contrust to `std::lock_guard`.
{% highlight c++ %}
std::unique_lock<std::mutex> ulocker(mu);
...
ulocker.unlock();
...
ulocker.lock();

//or
std::unique_lock<std::mutex> ulocker(mu, std::defer_lock);
...
ulocker.lock();
...
ulocker.unlock();

//or

std::unique_lock<std::mutex> locker(mu, std::adopt_lock);
...
ulocker.unlock();
...
ulocker.lock();
{% endhighlight %}
`unique_lock` is a little havier then `lock_guard`.

### Once flag
If we need to perform a function only once an in thread safe manner we can use `std::once_flag` flag and `std::call_once` function. In the example below the `shared_cout_2` will be called only once.
{% highlight c++ %}
#include <iostream>
#include <string>
#include <thread>
#include <mutex>

using namespace std;

std::mutex mu;
std::once_flag flag;

void shared_cout(string &&msg, int t_id)
{
    std::lock_guard<std::mutex> locker(mu);
    cout << "thread " << t_id << " msg: " << msg << endl;
}

void shared_cout_2(string &&msg, int t_id)
{
    std::unique_lock<std::mutex> ulocker(mu);
    cout << "thread " << t_id << " msg: " << msg << endl;
}

void thread_func(string &&msg, int t_id)
{
    for (int i = 0; i < 1000; i++)
    {
 	try
	{
            if (t_id == 2)
		std::call_once(flag, [&]() {shared_cout_2(std::move(msg), t_id); });
	    else
		shared_cout(std::move(msg), t_id);
	}
	catch (exception &ex)
	{
	    cout << "Exception is caught: " << ex.what() << endl;
	}
    }
}

int main()
{
    std::thread t1(thread_func, std::string("Hello world!"), 1);
    std::thread t2(thread_func, std::string("Another message!"), 2);

    t1.join();
    t2.join();
    return 0;
}
{% endhighlight %}

### Conditional variables
Suppose we have two worker threads, one of which supplies some data and the other does something with this data. The second thread needs to loop and check if there is data available or sleep. Pooling causes wasted processor's time and for sleep we don't know how much time we need to wait. A better solution is to use `Conditional variables`.  
{% highlight c++ %}
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <deque>

using namespace std;

std::deque<int> deq; // shared resource

std::mutex mu;
std::condition_variable cv;

void func1()
{
    int size = 100;
    while (size > 0)
    {
	std::unique_lock<std::mutex> locker(mu);   // lock the mutex
	cout << "t1: Push value " << size << endl;
	deq.push_back(size);
	locker.unlock(); // unlock the mutex after using the shared resource
	cv.notify_one(); // notify one thread waiting on this conditional variable

        size--;
     }
}

void func2()
{
    int size = 100;
    while (size > 0)
    {
 	std::unique_lock<std::mutex> locker(mu);       // lock the mutex
	cv.wait(locker, []() {return !deq.empty(); }); // wait until some other thread notifies about data readiness
	int val = deq.front();
	deq.pop_front();
	cout << "t2: Pop value " << val << endl;
	locker.unlock();                              // unlock the mutex after using the shared resource
	size--;
    }
}

int main()
{
    std::thread t1(func1);
    std::thread t2(func2);
    t1.join();
    t2.join();
    
    return 0;
}
{% endhighlight %}
The thread `t2` locks the mutex (if it is not locked by the other thread) and starts waiting on the `cv`. At this moment the mutex is unlocked (so other threads can access the shared resource while this thread waits for a notification). When thread `t1` notifies the thread `t2` through the `cv` the `t2` locks the mutex and accesses the shared resource. When waiting on a conditional variable thread can spontaneously wake up, to put him back to sleep we use a predicat (lambda in cv.wait()).

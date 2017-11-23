---
layout: post
comments: true
title:  "Type Casts Notes "
excerpt: "Brief summary of existing type cast methods in C++11: const_cast, static_cast, dynamic_cast and reinterpret_cast."
date:   2017-11-23 20:00:00
---

### const_cast
Typically used to cast away the constness of objects. It is the only C++ style that can do this.
```C++
const int *a = new int(5);
cout << "a = " << *a << endl;   // return 5
int *b = const_cast<int*>(a);   
cout << "b = " << *b << endl;   // return 5
*b = 9;
cout << "a = " << *a << endl;   // return 9
cout << "b = " << *b << endl;   // return 9
```

### reinterpret_cast
Converts any pointer type to any other pointer type, even of unrelated classes. The operation result is a simple binary copy of the value from one pointer to the other.  
Intended for low-level casts that yield implementation-dependent and it would not be portable.  
Let's consider an example:

```C++
struct Data {
    char a;
    char b;
};

int value = 261;
Data* pdata = reinterpret_cast<Data*> (&value);
cout << "a = " << pdata->a << ", b = " << pdata->b << endl;   //returns a = 5, b = 1
```
```
value = 261 = 0000 0000 0000 0000 0000 0001 0000 0101 = 4bytes (int)
a = 5 = 0000 0101 = 1byte (char)
b = 1 = 0000 0001 = 1byte (char)
```
So `reinterpret_cast` just read 1st and 2nd bytes of `value` and wrote it into `a` and `b`. It treats variable `value` of type `int` as a variable of type `struct Data`.

Another way of using `reinterpret_cast` is to compare pointers.
Here is an example:
```C++
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
```
`a == &c == true` - base pointer `a` points at `c`.  
`reinterpret_cast<char*>(a) == reinterpret_cast<char*>(&c) == true` - treats `a` and `&c` as pointers to `char`, the 1st byte is the same in both pointers.  
`b == &c == true` - base pointer `b` is also pointing at the `c` object.  
`reinterpret_cast<char*>(b) == reinterpret_cast<char*>(&c) == false` - after upcasting `b` kind of points at part of `c` and not at the beginning of it.  
`reinterpret_cast<char*>(a) == reinterpret_cast<char*>(b) == false` - after upcasting `a` points at one part of the `c` and `b` at the other.  
Output of the last string is:
```Adress of c = 000000E77974F664
a = 000000E77974F664
b = 000000E77974F665
```


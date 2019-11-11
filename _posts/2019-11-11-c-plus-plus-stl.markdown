---
layout: post
comments: true
title:  "C++ STL "
excerpt: "Brief overview of STL"
date:   2019-11-11 20:00:00
---

### Standard Library
STL is a combination of `algorithms` and `containers`. Algorithms access containers through `iterators`.
<div class="imgcap">
<img src="/assets/c-plus-pluc-stl/stl.jpg">
</div>

### Containers
### Sequence containers - emplemented as array of linked list.
1. `Vector` is a dynamically allocated contiguous array in memory. Can grow only in one side.  
Insert/Remove:  
O(1) - at the end.  
O(n) - at the begining/middle, because we need to shift the rest of the elements.  
Search: O(n).  
Random access: Yes.  

2. `Deque` is a dynamically allocated array (not contiguous) in memory, can grow in both sides.  
Insert/Remove:  
O(1) - at the begining and end.  
O(n) - at the middle, because we need to shift the rest of the elements.  
Search: O(n).  
Random access: Yes.  

3. `List` is a doubly linked list. It consists of not contiguous memory blocks. Consumes more memory as each element holds two pointers.   
Insert/Remove:  
O(1) - at any place (but if the position is not at the begining of end first we need to find it = O(n)).   
Search: O(n).  
Random access: No.  
Provides efficient function - splice O(1).  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

4. `Forward list` is a linked list.  

5. `Array`. Its size can not be changed.  

### Associative containers - binary tree (elements are sorted).  
They called `Associative` because elements consist of a `key` and a `value` associated with the `key`.  
1. `Map` - key, value pairs. No duplicated keys. Sorted by key.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No.  
Keys can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

2. `Multimap` - map which can contain duplicated keys.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No.  
Keys can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

`Set/Multiset` are special cases of Map/Multimap when key=value.  

3. `Set` - no duplicated items.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No.  
Values can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

4. `Multiset` - set which can contain duplicated items.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No.  
Values can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

### Unordered containers (Unordered set/multiset, Unordered map/multimap) - hash table.


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

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
<img src="/assets/c-plus-plus-stl/stl.jpg">
</div>

### Containers
### Sequence containers - emplemented as array or linked list.
1.`Vector` is a dynamically allocated contiguous array in memory. Can grow only in one side.
<div class="imgcap">
<img src="/assets/c-plus-plus-stl/stl_vector_container.jpg">
</div>
Insert/Remove:  
O(1) - at the end.  
O(n) - at the begining/middle, because we need to shift the rest of the elements.  
Search: O(n).  
Random access: Yes.  

2.`Deque` is a dynamically allocated array (not contiguous) in memory, can grow in both sides.  
Insert/Remove:  
O(1) - at the begining and end.  
O(n) - at the middle, because we need to shift the rest of the elements.  
Search: O(n).  
Random access: Yes.  

3.`List` is a doubly linked list. It consists of not contiguous memory blocks. Consumes more memory as each element holds two pointers.   
Insert/Remove:  
O(1) - at any place (but if the position is not at the begining of end first we need to find it = O(n)).   
Search: O(n).  
Random access: No.  
Provides efficient function - splice O(1).  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  

4.`Forward list` is a linked list.  

5.`Array`. Its size can not be changed.  

### Associative containers - binary tree (elements are sorted).  
They called `Associative` because elements consist of a `key` and a `value` associated with the `key`.  
1.`Map` - key, value pairs. No duplicated keys. Sorted by key.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: Yes.  Operator `[]` is implemented. Use `[key]` or `auto it = map_.find(key)` to get needed element.   
Keys can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  
{% highlight c++ %}
map<char, string> map_ = { {'M', "Monday"}, {'S', "Sunday"} };
cout << map_['M'] << endl;
{% endhighlight %}
2.`Multimap` - map which can contain duplicated keys.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No. Operator `[]` is not implemented. Use `auto it = map_.find(key)` to get needed element.   
Keys can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  
{% highlight c++ %}
multimap<char, string> mmap_ = { {'M', "Monday"}, {'S', "Sunday"}, {'M', "Monday2"}, {'T', "Thursday"} };
cout << mmap_.find('M')->second << endl; // returns the first 'M' entry
cout << mmap_.count('M') << endl;        // returns 2
auto iters = mmap_.equal_range('M');     // returns 2 iterators, first - to all elements with 'M' key, second - next after 'M' key
for (auto it = iters.first; it != iters.second; it++) // iterate over the first iterator to get all value with 'M' key
    cout << it->second << endl;
{% endhighlight %}
`Set/Multiset` are special cases of `Map/Multimap` when `key=value`.  

3.`Set` - no duplicated items.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
No. Operator `[]` is not implemented. Use `auto it = set_.find(value)` to get needed element.   
Values can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  
{% highlight c++ %}
set<char> set_ = { 'M', 'F', 'S' };
cout << *set_.find('F') << endl;
{% endhighlight %}
4.`Multiset` - set which can contain duplicated items.  
Insert/Remove:  
O(log(n)) - at any place.  
Search: O(log(n)).  
Random access: No. Operator `[]` is not implemented. Use `auto it = set_.find(value)` to get needed element.   
Values can not be modified.  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses.  
{% highlight c++ %}
multiset<char> mset_ = { 'M', 'F', 'S', 'M' };
cout << *mset_.find('S') << endl;
auto iters_set = mset_.equal_range('M');
for (auto it = iters_set.first; it != iters_set.second; it++)
    cout << *it << endl;
{% endhighlight %}
### Unordered containers - implemented as hash table.
<div class="imgcap">
<img src="/assets/c-plus-plus-stl/unordered_containers.jpg">
</div>

1.Unordered set - no duplicates, no `[]`; multiset - allows to contain duplicate elements, no `[]`.
{% highlight c++ %}
// unordered_set
unordered_set<string> s = { "red", "yellow", "blue" };
cout << *s.find("red") << endl;
  
//unordered_multiset
unordered_multiset<string> mset = { "Monday", "Sunday" };
cout << *(mset.find("Monday")) << endl;
{% endhighlight %}
2.Unordered map - no duplicates, `[]`; multimap - allows to contain duplicate keys, no `[]`.
{% highlight c++ %}
// unordered_map
unordered_map<char, string> unmap_ = { {'M', "Monday"}, {'S', "Sunday"} };
cout << unmap_['S'] << endl;

// unordered_multimap
unordered_multimap<char, string> mmap = { {'M', "Monday"}, {'S', "Sunday"} };
auto mmap_it = mmap.find('M');
cout << mmap_it->second << endl;
{% endhighlight %}
Insert/Remove:  
O(1) - at any place.  
Search: O(1).  
Values in sets and keys in maps can not be modified.  

Hash collision can degrade O(1) performance as several keys can go into one bucket and in order to keep it we need to insert it at the end of the linked list. The worse case when all elements go into one bucket:
<div class="imgcap">
<img src="/assets/c-plus-plus-stl/unordered_containers_degrade.jpg">
</div>

### Associative arrays - can be implemented as a map/unordered_map.
{% highlight c++ %}
unordered_map<char, string> arr = { {'M', "Monday"}, {'S', "Sunday"} };
cout << arr['M'] << endl;
cout << arr.at('S') << endl;  // perform boundary check, exception is thrown if no such key in the map
arr['F'] = string("Friday"); // add new element
{% endhighlight %}
`[]` operator returns a reference with write rights. `Multimap/unordered_multimap` can't be used to implement associative array because  they don't have `[]` operator.

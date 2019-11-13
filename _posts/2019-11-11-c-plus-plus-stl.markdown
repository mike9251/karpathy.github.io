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


## Iterators
### Random access iterators - vector, deque, array
{% highlight c++ %}
vector<int> vec = { 1,112,3,4,0,5,6,7 };
vector<int>::iterator itr1 = vec.begin();
auto itr2 = vec.begin();
itr1 = itr1 + 3;
itr2 = itr1 - 2;
if (itr1 > itr2) // compare indices not values
	cout << *itr1 << " > " << *itr2 << endl;
{% endhighlight %}
### Bidirectional iterators - list, set/multiset, map/multimap
{% highlight c++ %}
list<int> list_ = { 1,2,33,0,45 };
list<int>::iterator list_it = list_.begin();
list_it++;
list_it--;
{% endhighlight %}
### Forward iterators - forward list
{% highlight c++ %}
forward_list<int> flist_ = { 1,2,3,4,0,54,21 };
forward_list<int>::iterator flist_it = flist_.begin();
flist_it++;
{% endhighlight %}

Unordered containers provide at least forward iterators. Depends on the STL implementation it can provide Bidirectional iterators.  
Every container provides iterators and const iterators. Const iterators provide read-only rights to the container elements.
{% highlight c++ %}
vector<int> vec = { 1,112,3,4,0,5,6,7 };
vector<int>::const_iterator itr1 = vec.begin();
auto itr2 = vec.cbegin(); // cbegin() from C++11
{% endhighlight %}

Iterator functions (can be applied to non-random access iterators):
{% highlight c++ %}
advance(iterator, 5); //iterator+=5
distance(it1, it2);
{% endhighlight %}

### Iterator adoptors:
1.Insert iterator
{% highlight c++ %}
vector<int> vec1 = { 1,2 };
vector<int> vec2 = { 12,45,20,78,100 };
vector<int>::iterator it = find(vec2.begin(), vec2.end(), 20);
insert_iterator<vector<int>> i_it(vec2, it);
copy(vec1.begin(), vec1.end(), i_it);
for_each(vec2.begin(), vec2.end(), [](int x) {cout << x << " "; }); // 12 45 1 2 20 78 100
{% endhighlight %}
2.Stream iterator
{% highlight c++ %}
vector<string> str_vec;
copy(istream_iterator<string>(cin), istream_iterator<string>(), back_inserter(str_vec));
copy(str_vec.begin(), str_vec.end(), ostream_iterator<string>(cout, " "));
//or
copy(istream_iterator<string>(cin), istream_iterator<string>(), ostream_iterator<string>(cout, " "));
{% endhighlight %}
3.Reverse iterator
{% highlight c++ %}
vector<int> vec = { 1,2,3,4,5 };
vector<int>::reverse_iterator r_it = vec.rbegin();
for (auto r_iter = vec.rbegin(); r_iter != vec.rend(); r_iter++) // auto
{
    cout << *r_iter << " ";
}
//or
cout << endl;
for_each(vec.rbegin(), vec.rend(), [](int x) {cout << x << " "; });
{% endhighlight %}

## Algorithms
### Non-modifying Algorithms  
Almost all algorithms have generalized version - take one more argument - predicate.    
1.Counting algorithms
{% highlight c++ %}
vector<int> vec = { 10, 0, 2, 10, 10, 9, 120, 121, 89, 11, 1, 5,11,10,9,120,6,7,8,88 };
    
cout << count(vec.begin(), vec.end(), 10) << endl;
cout << count_if(vec.begin(), vec.end(), [](int x) { return x > 80; }) << endl;
{% endhighlight %}
2.Min/max element
{% highlight c++ %}
auto max_element_it = max_element(vec.begin(), vec.end());
auto min_element_it = min_element(vec.begin(), vec.end());
cout << *max_element_it << endl << *min_element_it << endl;
//or
auto min_max_it = minmax_element(vec.begin(), vec.end());
cout << *min_max_it.first << endl << *min_max_it.second << endl;
{% endhighlight %}
3.Linear search. Returns an iterator for the first matching element. If there is no match then returned iterator == vec.end()  
{% highlight c++ %}
auto it = find(vec.begin(), vec.end(), 11);
cout << (it == vec.end()) << endl;

auto cond_if = find_if(vec.begin(), vec.end(), [](int x) {return x == 11; }); // analog of the find 
cout << (cond_if == vec.end()) << endl;

auto cond_if_not = find_if_not(vec.begin(), vec.end(), [](int x) {return (x == 10)||(x == 0); }); // 2
cout << (cond_if_not == vec.end()) << endl;

auto search_el = search_n(vec.begin(), vec.end(), 2, 10); // search the first entry of a pattern 10, 10
cout << (search_el == vec.end()) << endl;

// finds the first subvec pattern match
vector<int> vec = { 10, 0, 2, 10, 10, 9, 120, 121, 89, 11, 1, 5,11,10,9,120,6,7,8,88 };
vector<int> subvec = { 10, 9, 120 };
auto search_sub = search(vec.begin(), vec.end(), subvec.begin(), subvec.end());
for (auto it = search_sub; it != vec.end(); it++)
	cout << *it << " "; // 10, 9, 120, 121, 89, 11, 1, 5,11,10,9,120,6,7,8,88
cout << endl;
// finds the last subvec pattern match
search_sub = find_end(vec.begin(), vec.end(), subvec.begin(), subvec.end());
for (auto it = search_sub; it != vec.end(); it++)
	cout << *it << " "; // 10,9,120,6,7,8,88

//search any_of
vector<int> any = { 1, 8 };
auto it = find_first_of(vec.begin(), vec.end(), any.begin(), any.end());
cout << *it << endl; // 1

auto adj_it = adjacent_find(vec.begin(), vec.end());
cout << *adj_it << " " << *(adj_it++) << endl; // 10, 10
{% endhighlight %}
4.Check attributes
{% highlight c++ %}
auto pred = all_of(vec.begin(), vec.end(), [](int x) {return x > 0; });
cout << pred << endl; // 0 - False, one element == 0
pred = any_of(vec.begin(), vec.end(), [](int x) {return x > 0; });
cout << pred << endl; // 1 - True, at least one element > 0
pred = none_of(vec.begin(), vec.end(), [](int x) {return x == -1; });
cout << pred << endl; // 1 - True, no elements == -1
{% endhighlight %}

### Modifying Algorithms  
1.Copy
{% highlight c++ %}
vector<int> vec = { 10,0,2,10,10,9,120,121,89,11,1,5,11,10,9,120,6,7,8,88 }; // 20
vector<int> vec2(22, 0);                                                     // 22

copy(vec.begin() + 3, vec.end() - 5, vec2.begin());
for_each(vec2.begin(), vec2.end(), [](int x) {if (x) cout << x << " "; });
// 10 10 9 120 121 89 11 1 5 11 10 9
cout << endl;

fill(vec2.begin(), vec2.end(), 0);
copy_backward(vec.begin(), vec.end(), vec2.end());
for_each(vec2.begin(), vec2.end(), [](int x) { cout << x << " "; });
// vec2: 0,0,10,0,2,10,10,9,120,121,89,11,1,5,11,10,9,120,6,7,8,88
cout << endl;
// or
fill(vec2.begin(), vec2.end(), 0);
copy(vec.begin(), vec.end(), vec2.begin() + 2); // need to be shure that vec2 has enough space to store vec
for_each(vec2.begin(), vec2.end(), [](int x) { cout << x << " "; });
// vec2: 0,0,10,0,2,10,10,9,120,121,89,11,1,5,11,10,9,120,6,7,8,88
cout << endl;

fill(vec2.begin(), vec2.end(), 0);
copy_if(vec.begin(), vec.end(), vec2.begin(), [](int x) { return x > 10; }); // need to be shure that vec2 has enough space to store vec
for_each(vec2.begin(), vec2.end(), [](int x) { cout << x << " "; });
// vec2: 120 121 89 11 11 120 88 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
cout << endl;

fill(vec2.begin(), vec2.end(), 0);
copy_n(vec.begin() + 2, 5, vec2.begin()); // need to be shure that vec2 has enough space to store vec
for_each(vec2.begin(), vec2.end(), [](int x) { cout << x << " "; });
// vec2: 2 10 10 9 120 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
cout << endl;
{% endhighlight %}
2.Move  
If a container holds objects of a class with `Move constructor` the `move` algorithm works using move semantic otherwise `copy` is performed.
{% highlight c++ %}
vector<string> vec = { "Red", "White", "Black", "Yellow" }; // 4
vector<string> vec2(6, "");                                 // 6

move(vec.begin(), vec.end(), vec2.begin());
for_each(vec2.begin(), vec2.end(), [](string &x) {cout << x << " "; });
// vec2: Red White Black Yellow
// vec: "" "" "" "" - empty

move_backward(vec.begin(), vec.end(), vec2.end());
for_each(vec2.begin(), vec2.end(), [](string &x) {cout << x << " "; });
// "" ""  "Red" "White" "Black" "Yellow"
{% endhighlight %}
3.Transform
{% highlight c++ %}
vector<int> vec  = { 1, 2, 3, 4 };
vector<int> vec2 = { 1, 2, 3, 4 };
vector<int> vec3(6, 0);

transform(vec.begin(), vec.end(), vec3.begin(), [](int x) {return x + 1; });
for_each(vec3.begin(), vec3.end(), [](int x) { cout << x << " "; });
// vec3: 2 3 4 5 0 0

transform(vec.begin(), vec.end(),                           // src1
			vec2.begin(),                       // src2
			vec3.begin(),                       //dst
			[](int x, int y) {return x + y; }); //operation
for_each(vec3.begin(), vec3.end(), [](int x) { cout << x << " "; });
//vec3: 2 4 6 8 0 0
{% endhighlight %}

4.Swap - two-way copying
{% highlight c++ %}
vector<int> vec  = { 1, 2, 3, 4 }; // 4
vector<int> vec2 = { 2, 4, 6, 8, 0, 0 }; // 6
swap_ranges(vec.begin(), vec.end(), vec2.begin());
for_each(vec2.begin(), vec2.end(), [](int x) { cout << x << " "; });
for_each(vec.begin(), vec.end(), [](int x) { cout << x << " "; });
// vec2: 1 2 3 4 0 0
// vec:  2 4 6 8
{% endhighlight %}

5.Fill
{% highlight c++ %}
vector<int> vec(10);
fill(vec.begin(), vec.end(), 5);
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 5 5 5 5 5 5 5 5 5 5

fill_n(vec.begin() + 2, 3, 23);
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 5 5 23 23 23 5 5 5 5 5

generate(vec.begin(), vec.end(), rand);
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 41 18467 6334 26500 19169 15724 11478 29358 26962 24464 - random

fill(vec.begin(), vec.end(), 5);
generate_n(vec.begin(), 5, rand);
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 5705 28145 23281 16827 9961 5 5 5 5 5
{% endhighlight %}

6.Replace
{% highlight c++ %}
vector<string> vec = { "Red", "Black", "Blue", "White" };
replace(vec.begin(), vec.end(), string("Blue"), string("Orange"));
for_each(vec.begin(), vec.end(), [](string &x) {cout << x << " "; });
// vec: Red Black Orange White

replace_if(vec.begin(), vec.end(), [](string &x) {return x.size() == 3; }, string("Gold"));
for_each(vec.begin(), vec.end(), [](string &x) {cout << x << " "; });
// vec: Gold Black Orange White

vector<string> vec2(4, "");
replace_copy(vec.begin(), vec.end(),  //src
	vec2.begin(),                 //dst
	string("White"),              //old value
	string("Green"));             //new value
for_each(vec2.begin(), vec2.end(), [](string &x) {cout << x << " "; });
// vec2: Gold Black Orange Green
{% endhighlight %}

7.Remove. Only _copy algorithms work. Don't know why.
{% highlight c++ %}
vector<int> vec = { 1, 1, 2, 3, 4 };
vector<int> vec2(5, 0);
remove_copy_if(vec.begin(), vec.end(), vec2.begin(), [](int x) {return x > 2; });
for (auto &i : vec2)
    cout << i << " "; // vec2: 1 1 2 0 0

unique_copy(vec.begin(), vec.end(), vec2.begin());
for (auto &i : vec2)
    cout << i << " "; // vec2: 1 2 3 4 0
{% endhighlight %}

8.Reverse
{% highlight c++ %}
vector<int> vec = { 1, 1, 2, 3, 4 };
vector<int> vec2(5, 0);

reverse_copy(vec.begin(), vec.end(), vec2.begin());
for_each(vec2.begin(), vec2.end(), [](int x) {cout << x << " "; });
// vec2: 4 3 2 1 1

reverse(vec.begin(), vec.end());
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 4 3 2 1 1
{% endhighlight %}

9.Rotate
{% highlight c++ %}
vector<int> vec = { 1, 1, 2, 3, 4 };
vector<int> vec2(5, 0);

rotate_copy(vec.begin(), vec.begin() + 3, vec.end(), vec2.begin());
for_each(vec2.begin(), vec2.end(), [](int x) {cout << x << " "; });
// vec2: 3 4 1 1 2

rotate(vec.begin(), vec.begin() + 3, vec.end());
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 3 4 1 1 2
{% endhighlight %}

10.Shuffle. Include <random> for default_random_engine.
{% highlight c++ %}
vector<int> vec = { 1, 1, 2, 3, 4 };

shuffle(vec.begin(), vec.end(), default_random_engine(1));
for_each(vec.begin(), vec.end(), [](int x) {cout << x << " "; });
// vec: 3 1 2 4 1
{% endhighlight %}

## String
## Stream
C++ Input/Output library.  
Stream is a serial IO interface to external devices (file, stdin/stdout, network, etc.).  


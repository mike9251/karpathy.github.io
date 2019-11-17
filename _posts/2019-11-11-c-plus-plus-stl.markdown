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
When there is no space to push_back a new element the vector is reallocated (usually x2 or x1.5), old elements get copyed and the new element is added.

2.`Deque` is a dynamically allocated array (not contiguous) in memory, can grow in both sides. It can be implemented as array of arrays - the outter array contains pointers to inner arrays which hold data. When we want to push_back or push_front an element and there is no space in the current inner vector we do not reallocate old vectors but allocate new contiguous blocks of memory (arrays) and put its pointer in the outter vector (at the front/back of the deque) and place the new element.
Insert/Remove:  
O(1) - at the begining and end.  
O(n) - at the middle, because we need to shift the rest of the elements.  
Search: O(n).  
Random access: Yes.  
Data is not contiguous - slightly slower then vectors. But if data type is complex - probably managing objects of such data type is more consumming, in this case `Deque` is preferable (no copying needed).  

Recomendation:  
1. Frequent `push_front` - deque
2. Build-in data type - vector
3. Not build-in data type - deque
4. Unpredictable growth - deque
5. Frequently passed to C functions - vector (as we can get access to elements of the vector by int *p = &vec[0]

### Vector vs Deque

3.`List` is a doubly linked list. It consists of not contiguous memory blocks. Consumes more memory as each element holds two pointers.   
Insert/Remove:  
O(1) - at any place (but if the position is not at the begining of end first we need to find it = O(n)).   
Search: O(n).  
Random access: No.  
Provides efficient function - splice O(1).  
Traversing is slow comparing to vectors or deques. Because of more frequent cache-misses (data is not contiguous).  

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
File system stream:
{% highlight c++ %}
#include <fstream>
int main()
{
    {
        ofstream f("MyFile.txt");  // open and clean an existing file or create a new file.
        f << "12345";
	f.flush();                 // writes all data in ofstream into file
    }
    //MyFile.txt : 12345
    
    {
        ofstream f("MyFile.txt");  // open and clean an existing file or create a new file.
        f << "12345" << endl;      // endl puts the \0 symbol and calls flash() method.         
    }
    //MyFile.txt : 12345\0
    
    {
        ofstream f("MyFile.txt", ofstream::app);  // open an existing file or create a new file. Existing content will be saved and new 						  // data will be added at the end of the file 
        f << "1234567890";
	f.flush();                 // writes all data in ofstream into file
    }
    //MyFile.txt : 12345\0
    //             1234567890
    
    {
        ofstream f("MyFile.txt", ofstream::in | ofstream::out); // allow to find a place to start writing
	f.seekp(3, ofstream::beg);                              // writing position = 3 symbols from the begining
	f << "000000";                                          // starting at position [3] write 000000, replacing existing symbols
	f.flush();
    }
    //MyFile.txt : 12300000034567890

    return 0;
}
{% endhighlight %}
Formatted input - read a file and treat its data with respect to variable type.
{% highlight c++ %}
#include <fstream>
int main()
{
    ifstream f("E:\\MyFile.txt");
    int i;
    f >> i;
    if (f.good()) // if first element is some number in int range
    {
	cout << "OK\n";
    }
    else if (f.bad())
    {
 	cout << "Failed! Recovery is impossible\n";
    }
    else if (f.fail())
    {
        cout << "Failed! Recovery is possible\n";
    }
    else if (f.eof())
    {
 	cout << "EOF\n";
    }
    
    f.clear(); // clear error bit

    return 0;
}
{% endhighlight %}

Formatting:
{% highlight c++ %}
cout << 34 << endl; // 34
cout.setf(ios::hex, ios::basefield);
cout << 34 << endl; // 0x22

cout.setf(ios::dec, ios::basefield);
cout.setf(ios::oct, ios::basefield);

cout.unsetf(ios::showbase);
{% endhighlight %}

Unformatted IO - tread all data as text, read/write char symbols.
{% highlight c++ %}
ifstream f("MyFile.txt");
char buf[80];
//input
f.get(buf, 80);     // reads up to 80 characters
f.getline(buf, 80); // reads up to 80 characters or until the \n encountered

//output
ofstream of("MyFileOut.txt");
of.put('c');
of.write(buf, 6); // writes 6 first symbols of buf
of.flush();       // flush the data into output file
{% endhighlight %}

### std::endl
{% highlight c++ %}
ostream& endl(ostream &osm)
{
    osm.put('\n');
    osm.flush();
    return osm;
}
{% endhighlight %}

### Stream buffer
We can create custom IO streams (override). Example, redirecting:
{% highlight c++ %}
ofstream f("E:\\MyFile.txt");  // create ofstream
streambuf *buf = f.rdbuf();    // get ofstream buffer, where data is written before flush
streambuf *orig_cout_stream_buffer = cout.rdbuf(); // in order to recover cout
cout.rdbuf(buf);               // set custom stream buffer to the cout
cout << "12345" << endl;       // write data into stream buffer and flush it into file

cout.rdbuf(orig_cout_stream_buffer); // restore default cout
{% endhighlight %}

### Member functions vs Algorithms
In most cases member functions are preferable over algorithms, because they know about underlaying data structure and can take advantage of it. Examples:  
Find  
{% highlight c++ %}
unordered_set<int> unset = { 1,8,3,6 };     // hash table
// Member function
auto it = unset.find(3);                    // O(1)
// Algorithm
it = find(unset.begin(), unset.end(), 3);   // O(n)
{% endhighlight %}
{% highlight c++ %}
unordered_set<int> unset = { 1,8,3,6 };     // hash table
auto it = unset.find(3);                    // O(1)
it = find(unset.begin(), unset.end(), 3);   // O(n)
{% endhighlight %}
Remove  
{% highlight c++ %}
list<int> list_ = { 3, 1, 5, 7, 0, 4 };
list_.remove(7);              // O(n): O(n) to find the element, O(1) actual removind and relinking
// list_: 3 1 5 0 4
// Algorithm
auto it = remove(list_.begin(), list_.end(), 7);  // O(n): removes by copying - it can be very expensive if elements are complex
// list_: 3 1 5 0 4 4
list_.erase(it);
// list_: 3 1 5 0 4
{% endhighlight %}
	
### Reverse iterators
We can get a reverse iterator from a standard iterator:
{% highlight c++ %}
vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
vector<int>::iterator it;
vector<int>::reverse_iterator rit;

it = find(vec.begin(), vec.end(), 6);
cout << *it << endl; // 6

rit = find(vec.rbegin(), vec.rend(), 6);
cout << *rit << endl; // 6

it = rit.base();
cout << *it << endl;  // 7 - rit moved one element to the right

rit = vector<int>::reverse_iterator(it);
cout << *rit << endl; //6 - it moved one element to the left
	
{% endhighlight %}
<div class="imgcap">
<img src="/assets/c-plus-plus-stl/reverse-iterator.png">
</div>
When we convert a revese iterator to an iterator (`base() member function`), the result iterator will be the reverse iterator moved one element to the right. When we convert an iterator to a reverse iterator (`vector<int>::revers_iterator(it)`), the result reverse iterator will be the iterator moved one element to the left.

### Remove operation
We can remove elements from a container using algorithms or member functions.
<div class="imgcap">
<img src="/assets/c-plus-plus-stl/stl-list-remove.png">
</div>
Vectors don't have remove member function:
{% highlight c++ %}
vector<int> vec = {1, 2, 3, 6, 4, 5, 6, 7, 8, 9, 0};
cout << "Capacity: " << vec.capacity() << endl; // 11
vector<int>::iterator it = remove(vec.begin(), vec.end(), 6); // remove by copying
// vec: 1, 2, 3, 4, 5, 7, 8, 9, 0, 0 (it), 0
vec.erase(it, vec.end());   // capacity is still 11
// vec: 1, 2, 3, 4, 5, 7, 8, 9, 0
vector<int>(vec).swap(vec);   // capacity 9
//or
vec.shrink_to_fit(); // C++11
{% endhighlight %}

Lists have an efficient `remove` member function, O(n) to find elements and O(1) to relink elements:
{% highlight c++ %}
list<int> list_ = { 1,2,3,6,4,5,6,7 };
list_.remove(6);
// list_: 1,2,3,4,5,7

//or
auto iter = remove(list_.begin(), list_.end(), 6);
list_.erase(iter, list_.end());
// list_: 1,2,3,4,5,7 but it removes by copying - not so efficient
{% endhighlight %}

Removing elements from ordered associative containers (map/multimap, set/multiset) can be done in O(log(n)) by using `erase` member function (instead of O(n) using `remove` algorithm):
{% highlight c++ %}
map<int, string> map_ = { {1, "one"}, {2, "two"}, {5, "five"} };
map_.erase(2);
// map_: {1, "one"}, {5, "five"}
{% endhighlight %}

1. Vector od Deque: `remove` algorithm + `erase` member function to remove redundant elements.
2. List: member function `remove`.
3. Associative containers (ordered/unordered): member function `erase`.

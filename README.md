# t_standard
Data structures and algorithms

## Features
* All classes are designed non-copyable
* Contains lock-free atomic classes for multi-threading programming *(unfinished)*

# Menu
### **Classes**
* ### basic classes
    * [NonMoveable](#NonMoveable) : interface for non-moveable classes
    * [NonCopyable](#NonCopyable) : interface for non-copyable classes

* ### Template classes
    * [FixedArr](#FixedArr) : simple fixed sized array type
    * [DynamicArr](#DynamicArr) : array that with dynamic size
        * [ArrBuffer](#ArrBuffer) : array that can manage its storage and grow
            * [ByteBuffer](#ByteBuffer) : buffer type that designed to store the bytes data
                * [String](#String) : string type
                * [ByteStream](#ByteStream) : stream type with some operators
    * [SingleNode](#SingleNode)
    * [DoubleNode](#DoubleNode)
    * [LinkList](#LinkList) : fast and realiable LinkList template class
        * [List](#List) : DoubleNode LinkList
        * [Queue](#Queue) : SingleNode LinkList
        * [Stack](#Stack) : SingleNode LinkList

* ### Iter classes
    * [t_iter_traits.hpp](#t_iter_traits.hpp)
    * [LinkListIter](#LinkListIter)
    * [LinkListIterConst](#LinkListIterConst)

* ### Atomic classes
    * [AtomicNode](#AtomicNode) *(unfinished)*
    * [AtomicQueue](#AtomicQueue) *(unfinished)*
    * [AtomicStack](#AtomicStack) *(unfinished)*

* ### **Meta functions & structs & constexprs**
    * [t_meta.hpp](#t_meta\.hpp)

* ### **Algorithms**
    * [t_math.hpp](#t_meta\.hpp) : math functions
    * [t_sort.hpp](#t_meta\.hpp) : sort functions



# LinkList
> *!!! Code that be contained in the README.md may not be the latest version*
### Definition
```c++
template<typename Node_t>
requires (__is_node_t<Node_t>)
class LinkList : public NonCopyble;
```
### Type Definition
```C++
using T = Node_t::ContentType;
using ContentType = T;
```

### Constructors
```C++
LinkList() noexcept;

LinkList(LinkList&& _other)noexcept;

LinkList(std::initializer_list<T> const& _ini_list) noexcept;
```

### operators
```C++
LinkList& operator=(LinkList&& _other) noexcept;
```
Access to the items
```C++
T& operator[](usize _pos);
T const& operator[](usize _pos) const;
```

### Public functions
Clone or change the ownership
```C++
LinkList copy() const noexcept requires(std::is_copy_constructible_v<T>);

virtual LinkList&& move() noexcept;
```
Access to the items
```C++
LinkListIter<Node_t> begin() noexcept;
LinkListIterConst<Node_t> begin()const noexcept;

LinkListIter<Node_t> end() noexcept;
LinkListIterConst<Node_t> end()const noexcept;

LinkListIter<Node_t> seek(usize _pos) noexcept;
LinkListIterConst<Node_t> seek(usize _pos) const noexcept;

LinkListIter<Node_t> rbegin() noexcept requires(__is_DoubleNode_t<Node_t>);
LinkListIterConst<Node_t> rbegin() const noexcept requires(__is_DoubleNode_t<Node_t>);


LinkListIter<Node_t> rend() noexcept requires(__is_DoubleNode_t<Node_t>)
LinkListIterConst<Node_t> rend() const noexcept requires(__is_DoubleNode_t<Node_t>);


LinkListIter<Node_t> find(T const& _tar) noexcept requires(__is_eq_comparable<T>)
LinkListIterConst<Node_t> find(T const& _tar) const noexcept requires(__is_eq_comparable<T>)

LinkListIter<Node_t> last() noexcept;
LinkListIterConst<Node_t> last() const noexcept;

```
Modify the item
```C++
// modify the head or tail

// push
template<typename ...Args> void push_back(Args&& ... _data);
template<typename ...Args> void push_front(Args&& ... _data);
// pop
T pop_back();
T pop_front();

// modify any other item

// insert
template<typename ...Args>void insert_back(LinkListIter<Node_t> const& _iter,Args&& ... _data) noexcept;
template<typename ...Args> requires(__is_DoubleNode_t<Node_t>) void insert_front(LinkListIter<Node_t> const& _iter,Args&& ... _data) noexcept;
// erase
template<typename IterType> bool erase(IterType&& _iter) noexcept requires(__is_DoubleNode_t<Node_t>);
```

**Give**
> Give functions are designed to move a node from one LinkList to another Link list (also supports from self to self)
```C++
template<typename IterType_1 , typename IterType_2> void give_to_front(IterType_1&& from_this, IterType_2&& to_other) requires(__is_DoubleNode_t<Node_t>);

template<typename IterType_1 , typename IterType_2> void give_to_back(IterType_1&& from_this, IterType_2&& to_other) requires(__is_DoubleNode_t<Node_t>);

// swap the data of two iters
template<typename IterType_1 , typename IterType_2> static void swap(IterType_1&& _1, IterType_2&& _2)
```

Other functions
```C++
usize size() const noexcept;
```


# License
MIT License

Copyright (c) 2024 Cenxuan

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.



# Contact
If you have any questions or suggestions, feel free to contact me at my email: 
<br>
*<cenxuan.tian@outlook.com>*
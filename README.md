# t_standard
Data structures and algorithms based on C++ 20 standard
> *!!! Code that be contained in the README.md may not be the latest version*

## Features
* Head-only
* All classes are designed non-copyable
* Contains lock-free atomic classes for multi-threading programming *(unfinished)*


# Menu
### **Meta functions & structs & constexprs**
* [t_meta.hpp](#t_meta\.hpp)

### **Algorithms**
* [t_math.hpp](#t_meta\.hpp) : math functions
* [t_sort.hpp](#t_meta\.hpp) : sort functions

### **Template Classes**
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
    * [AtomicRingQueue](#AtomicRingQueue) Thread Safe ring buffer
    * [AtomicQueue](#AtomicQueue) *(unfinished)*
    * [AtomicNode](#AtomicNode) *(unfinished)*
    * [AtomicStack](#AtomicStack) *(unfinished)*



# NonMoveable
### Definition (NonMoveable)
interface for non-moveable classes
```c++
class NonMoveable;
```

# NonCopyable
interface for non-copyable classes
### Definition (NonCopyable)
```c++
class NonCopyable;
```

# FixedArr
### Definition (FixedArr)
```c++
template<typename T,usize _s> using FixedArr = T[_s];
```

# DynamicArr
* [Definition](#definition-dynamicarr)
* [Type Definition](#type-definition-dynamicarr)
* [Constructors](#constructors-dynamicarr)
* [Operators](#operators-dynamicarr)
* [Public functions](#public-functions-dynamicarr)
    * [Clone or change the ownership](#clone-or-change-the-ownership-dynamicarr)
    * [Access to items](#access-to-items-dynamicarr-1)
* [Other functions](#other-functions-dynamicarr)
***
### Definition (DynamicArr)
```C++
template<typename T>
class DynamicArr:public NonCopyble;
```

### Type Definition (DynamicArr)
```C++
using ContentType = T;
```
### Constructors (DynamicArr)
```C++
// default constructor
DynamicArr() noexcept;
// move constructor
DynamicArr(DynamicArr && _other) noexcept;
// construct by c array
DynamicArr(const T* _data,usize _space)noexcept;
// construct by initializer_list
DynamicArr(std::initializer_list<T> const& _ini_list) noexcept;
```
### Operators (DynamicArr)
#### Move asign (DynamicArr)
```C++
DynamicArr& operator=(DynamicArr && _other) noexcept;

T& operator[](usize _pos);
T const& operator[](usize _pos) const;

bool operator==(DynamicArr<T> const& _other) const noexcept
```
### Public functions (DynamicArr)
#### Clone or change the ownership (DynamicArr)
```C++
DynamicArr<T> copy() const noexcept;
virtual DynamicArr<T>&& move() noexcept;
```
#### Access to items (DynamicArr)
```C++
// access to all
T const* data()const noexcept;
T* data() noexcept;

// iter
DynamicArrIter<T> begin();
DynamicArrIterConst<T> begin()const;
DynamicArrIter<T> end();
DynamicArrIterConst<T> end()const;
```
### Other functions (DynamicArr)
```C++
const usize size() const noexcept;
```


# ArrBuffer
* [Definition](#definition-arrbuffer)
* [Constructors](#constructors-arrbuffer)
* [Public functions](#public-functions-arrbuffer)
    * [Size](#size-arrbuffer)
    * [Access items](#access-items-arrbuffer)
    * [Modify items](#modify-items-arrbuffer)
***
### Definition (ArrBuffer)
```C++
// this class can grow up and reserve size by itself
template<typename T>
class ArrBuffer :public DynamicArr<T>;
```
### Constructors (ArrBuffer)
```C++
ArrBuffer()noexcept;
ArrBuffer(const T* _data,usize _space)noexcept;
ArrBuffer(std::initializer_list<T> const& _ini_list) noexcept;
ArrBuffer(ArrBuffer<T> && _other) noexcept;
ArrBuffer(DynamicArr<T>&& _other)noexcept;
```
### Public functions (ArrBuffer)
#### Size (ArrBuffer)
```C++
// get the size of valid data
const usize size() const noexcept;

// get the size of space allocated
const usize capability() const noexcept;

// modify the capability
void reserve_by(usize _size) noexcept;
void reserve_to(usize _size);
```

### Access items (ArrBuffer)
> Functions extends from DynamicArr also can be used for this class
```C++
DynamicArrIter<T> seek(usize _pos) noexcept;

// just get the view of an item
// then move the iter forward
const T* view_and_move_forward(DynamicArrIter<T>& _iter, usize _size);
```

### Modify items (ArrBuffer)
```C++
// push back single
ArrBuffer& push_back(T const& _c) noexcept;

// push back multi
ArrBuffer& push_back(ArrBuffer const& _another) noexcept;

// push back
template<typename _InputType>
requires( std::is_same_v<_InputType,ArrBuffer<T>> ||  std::is_base_of_v<ArrBuffer<byte>,T>) 
ArrBuffer& operator+=(_InputType const& _in_data) noexcept;

// push back reture new object
template<typename _InputType>
requires( std::is_same_v<_InputType,ArrBuffer<T>> || std::is_base_of_v<ArrBuffer<byte>,T>)
ArrBuffer operator+(_InputType const& _in_data) noexcept;

// push front single
ArrBuffer& push_front(T const& _c) noexcept;

// push front multi
ArrBuffer& push_front(ArrBuffer const& _another) noexcept;

// tail operation
bool read_from_tail(T* _dist, usize _size) const;
bool erase_from_tail(usize _size) noexcept;

// head operation
bool read_from_head(T* _dist, usize _size) const;
bool erase_from_head(usize _size) noexcept;

// clear or reset
void clear() noexcept;
void zeromem() noexcept;
```



# LinkList
* [Definition](#definition-linklist)
* [Type Definition](#type-definition-linklist)
* [Constructors](#constructors-linklist)
* [Operators](#operators-linklist)
    * [move asign](#move-assign-linklist)
    * [Access to items](#access-to-items-linklist)
* [Public functions](#public-functions-linklist)
    * [Clone or change the ownership](#clone-or-change-the-ownership-linklist)
    * [Access to items](#access-to-items-linklist-1)
    * [Modify items](#modify-items-linklist)
    * [Give](#give-linklist)
* [Other functions](#other-functions-linklist)


***
### Definition (LinkList)
```c++
template<typename Node_t>
requires (__is_node_t<Node_t>)
class LinkList : public NonCopyble;
```
### Type Definition (LinkList)
```C++
using T = Node_t::ContentType;
using ContentType = T;
```

### Constructors (LinkList)
```C++
LinkList() noexcept;

LinkList(LinkList&& _other)noexcept;

LinkList(std::initializer_list<T> const& _ini_list) noexcept;
```

### Operators (LinkList)
#### Move assign  (LinkList)
```C++
LinkList& operator=(LinkList&& _other) noexcept;
```
#### Access to items  (LinkList)
```C++
T& operator[](usize _pos);
T const& operator[](usize _pos) const;
```

### Public functions (LinkList)
#### Clone or change the ownership  (LinkList)
```C++
LinkList copy() const noexcept requires(std::is_copy_constructible_v<T>);

virtual LinkList&& move() noexcept;
```
#### Access to items  (LinkList)
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


#### Modify items  (LinkList)
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

#### Give (LinkList)
> Give functions are designed to move a node from one LinkList to another Link list (also supports from self to self)
```C++
template<typename IterType_1 , typename IterType_2> void give_to_front(IterType_1&& from_this, IterType_2&& to_other) requires(__is_DoubleNode_t<Node_t>);

template<typename IterType_1 , typename IterType_2> void give_to_back(IterType_1&& from_this, IterType_2&& to_other) requires(__is_DoubleNode_t<Node_t>);

// swap the data of two iters
template<typename IterType_1 , typename IterType_2> static void swap(IterType_1&& _1, IterType_2&& _2)
```

#### Other functions (LinkList)
```C++
usize size() const noexcept;
```





# AtomicRingQueue
> Thread Safe ring buffer, which can just be modified in the head and tail. If the buffer is full, the data in the head will be overwriten.

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
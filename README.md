# t_standard
Data structures and algorithms based on C++ 20 standard
> *!!! Code that is contained in the README.md may not be the latest version*

## Features
* Head-only
* All classes are designed non-copyable
* Contains lock-free atomic classes for multi-threading programming *(unfinished)*


# Menu
### **Meta functions & structs & constexprs**
* [t_meta.hpp](../t_standard/basic/t_meta.hpp) : Useful meta funtions and variables
* [t_coroutine.hpp](../t_standard/basic/t_coroutine.hpp) : Useful Coroutine templates

### **Algorithms**
* [t_math.hpp](../t_standard/algorithm/t_math.hpp) : math functions
* [t_sort.hpp](../t_standard/algorithm/t_sort.hpp) : sort functions

### **Classes**
* ### basic classes
    * [NonMoveable](#NonMoveable) : interface for non-moveable classes
    * [NonCopyable](#NonCopyable) : interface for non-copyable classes
    * [t_bit.hpp](#t_bit.hpp) : contains useful bit structures and unions

* ### [Template classes](./docs/template.md#Template-Classes)
    * [FixedArr](./docs/template.md#FixedArr) : simple fixed sized array type
    * [DynamicArr](./docs/template.md#DynamicArr) : array that with dynamic size
        * [ArrBuffer](./docs/template.md#ArrBuffer) : array that can manage its storage and grow
            * [ByteBuffer](./docs/template.md#ByteBuffer) : buffer type that designed to store the bytes data
                * [String](./docs/template.md#String) : string type
                * [ByteStream](./docs/template.md#ByteStream) : stream type with some operators
    * [SingleNode](./docs/template.md#SingleNode)
    * [DoubleNode](./docs/template.md#DoubleNode)
    * [BinaryTreeNode](./docs/template.md#BinaryTreeNode)
    * [LinkList](./docs/template.md#LinkList) : fast and reliable LinkList template class
        * [List](./docs/template.md#List) : DoubleNode LinkList
        * [Queue](./docs/template.md#Queue) : SingleNode LinkList
        * [Stack](./docs/template.md#Stack) : SingleNode LinkList
    * [SearchBinaryTree](./docs/template.md#SearchBinaryTree) : Binary Search Tree / Ordered list


* ### Iter classes
    * [t_tree_iter.hpp](#t_tree_iter.hpp) : C++ 20 Coroutine based iter
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

# AtomicRingQueue
> Thread Safe ring buffer, which can only be modified in the head and tail. If the buffer is full, the data in the head will be overwritten.
### Definition (AtomicRingQueue)
```C++
template<typename _Type,size_t _Size>
class AtomicRingQueue: public NonCopyble;
```

### Public functions (AtomicRingQueue)
```C++
void push(_Type const& _in_ldata);

size_t safe_pop_pos() noexcept;
_Type safe_pop(size_t pos) noexcept;

// this function can only get the current size
// If other thread has called "push" or "safe_pop_pos" function
// The size could be different
size_t size()const noexcept;

size_t capacity()const noexcept;

void clear() noexcept;

// This function will not really clear the data.
// It will just change the marks of the head and tail position
// So this function is fast
void clear() noexcept;
```

### Examples (AtomicRingQueue)
```C++
// create the object
AtomicRingQueue<int,100> q;
// Push something into the queue
q.push(1);

// try to pop
auto pos = q.safe_pop_pos();
// Check if the pos is right
if(pos!=0ULL){
    // Read the data out
    int front = q.safe_pop(pos);
}else{
    // The queue is empty currently
}
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

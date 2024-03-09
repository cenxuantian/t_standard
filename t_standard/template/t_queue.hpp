#pragma once
#include "t_linklist.hpp"

namespace tcx
{
template<typename T>
class Queue: public LinkList<SingleNode<T>>{
public:
    using Node_t = SingleNode<T>;
    using Base_t = LinkList<SingleNode<T>>;

    Queue() noexcept :Base_t(){}
    ~Queue(){};
    Queue(Queue&& _other) :Base_t(std::forward<Queue&&>(_other)){}
    
    template<typename InputType>
    requires(std::is_base_of_v<Base_t,InputType> || std::is_same_v<Base_t,InputType>)
    Queue(InputType&& _other):Base_t(std::forward<InputType&&>(_other)){}

    Queue(std::initializer_list<T> const& _ini_list) :Base_t(_ini_list){}

    LinkListIter<Node_t> begin() noexcept = delete;
    LinkListIter<Node_t> end() noexcept = delete;
    LinkListIterConst<Node_t> begin()const noexcept = delete;
    LinkListIterConst<Node_t> end()const noexcept = delete;
    LinkListIter<Node_t> seek(usize _pos) noexcept = delete;
    LinkListIterConst<Node_t> seek(usize _pos) const noexcept = delete;
    template<typename ...Args> void push_back(Args&& ... _data) = delete;
    template<typename ...Args> void push_front(Args&& ..._data) = delete;
    T pop_front() = delete;
    T pop_back() = delete;
    T& operator[](usize _pos) = delete;
    T const& operator[](usize _pos)const = delete;
    template<typename ...Args> void insert_back(LinkListIter<Node_t> const& _iter,Args&& ... _data) noexcept = delete;

    template<typename ...Args>
    Queue& push(Args&& ... _data) 
    noexcept(noexcept(this->Base_t::push_back(std::forward<Args&&>(_data)...)))
    {
        this->Base_t::push_back(std::forward<Args&&>(_data)...);
        return *this;
    }

    void pop() 
    noexcept(noexcept(this->Base_t::pop_front()))
    {
        this->Base_t::pop_front();
    }

    T& front()
    {
        return *(this->root_->data_);
    }

    const T& front() const
    {
        return *(this->root_->data_);
    }

    bool empty() const noexcept{
        return this->size_ == 0;
    }
};
} // namespace tcx

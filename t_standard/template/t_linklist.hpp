#pragma once
#include "t_node.hpp"
#include "../basic/t_class_helper.h"
#include <stdexcept>

namespace tcx
{
template<typename Node_t>
requires (__is_node_t<Node_t>)
class LinkList ;

template<typename Node_t>
class LinkListIter : public TemplateIterTraits<LinkList<Node_t>>{
public:
    friend class LinkList<Node_t>;

    using T = LinkList<Node_t>::T;

    using TraitsType = TemplateIterTraits<LinkList<Node_t>>;
    using OwnerType = LinkList<Node_t>;
    using ContentType = LinkList<Node_t>::ContentType;

    using MoveFuncType = void(*)(Node_t*&);

    LinkListIter(LinkList<Node_t> & owner_, Node_t* _cur, MoveFuncType _move_func)
        noexcept :TraitsType(owner_),cur_(_cur),move_func_(_move_func){};

    virtual LinkListIter& operator++()override{
        move_func_(cur_);
        // cur_ = cur_->child_;
        return *this;
    };
    virtual T& operator*() override {
        return *(cur_->data_);
    };
    virtual bool operator!=(TraitsType const& _other) override{
        LinkListIter const& _other_1 = dynamic_cast<LinkListIter const&>(_other);
        return _other_1.cur_ != cur_;
    }

private:
    Node_t* cur_;
    MoveFuncType move_func_;
};

template<typename Node_t>
class LinkListIterConst : public TemplateIterConstTraits<LinkList<Node_t>>{
public:
    friend class LinkList<Node_t>;

    using T = LinkList<Node_t>::T;

    using TraitsType = TemplateIterConstTraits<LinkList<Node_t>>;
    using OwnerType = LinkList<Node_t>;
    using ContentType = LinkList<Node_t>::ContentType;

    using MoveFuncType = void(*)(Node_t const*&);

    LinkListIterConst(LinkList<Node_t> const& owner_, const Node_t* _cur, MoveFuncType _move_func)
    noexcept :TraitsType(owner_),cur_(_cur),move_func_(_move_func){};

    virtual LinkListIterConst& operator++()override{
        move_func_(cur_);
        // cur_ = cur_->child_;
        return *this;
    };
    virtual T const& operator*() override {
        return *(cur_->data_);
    };
    virtual bool operator!=(TraitsType const& _other) override{
        LinkListIterConst const& _other_1 = dynamic_cast<LinkListIterConst const&>(_other);
        return _other_1.cur_ != cur_;
    }

private:
    const Node_t* cur_;
    MoveFuncType move_func_;
};

template<typename Node_t>
requires (__is_node_t<Node_t>)
class LinkList : public NonCopyble{
public:
    using T = Node_t::ContentType;
    using ContentType = T;

// constructors & move & copy functions
    LinkList() noexcept :root_(nullptr),size_(0),last_(nullptr){}
    
    // O(n) !!! depends on the count of children
    ~LinkList(){
        __free_node_and_child(root_);
    }

    // O(1)
    LinkList(LinkList&& _other)noexcept : root_(_other.root_),size_(_other.size_),last_(_other.last_){
        _other.__leak();
    }

    // O(n) !!!
    LinkList(std::initializer_list<T> const& _ini_list) noexcept:root_(nullptr),size_(0),last_(0){
        for(const auto&i :_ini_list){
            push_back(i);
        }
    }

    // O(n) !!!
    // n = the size of this LinkList while this operator called
    LinkList& operator=(LinkList&& _other) noexcept{
        __free_node_and_child(root_);
        this->root_ = _other.root_;
        this->size_ = _other.size_;
        this->last_ = _other.last_;
        _other.__leak();
        return *this;
    }

    // O(n) !!!
    LinkList copy()
    const noexcept 
    requires(std::is_copy_constructible_v<T>)
    {
        LinkList res;
        const auto& _this = *this;
        for(const auto&i:_this){
            res.push_back(i);
        }
        
        return res.move();
    }

    // O(1)
    virtual LinkList&& move() noexcept{
        return std::move(*this);
    }


// functions to get the iter or access the Linklist

    // O(1)
    LinkListIter<Node_t> begin() noexcept{
        return {*this,root_,[](Node_t*& cur){cur = cur->child_;}};
    }

    // O(1)
    LinkListIter<Node_t> end() noexcept{
        return {*this,nullptr,[](Node_t*& cur){cur = cur->child_;}};
    }

    // O(1)
    LinkListIterConst<Node_t> begin()const noexcept{
        return {*this,root_,[](const Node_t*& cur){cur = cur->child_;}};
    }

    // O(1)
    LinkListIterConst<Node_t> end()const noexcept{
        return {*this,nullptr,[](const Node_t*& cur){cur = cur->child_;}};
    }

    // O(n) !!!
    LinkListIter<Node_t> seek(usize _pos) noexcept{
        Node_t* cur = this->root_;
        usize cur_pos = 0;
        while(cur && cur_pos<_pos){
            ++cur_pos;
            cur = cur->child_;
        }
        return {*this,cur,[](Node_t*& cur){cur = cur->child_;}};
    }

    // O(n) !!!
    LinkListIterConst<Node_t> seek(usize _pos) const noexcept{
        const Node_t* cur = this->root_;
        usize cur_pos = 0;
        while(cur && cur_pos<_pos){
            ++cur_pos;
            cur = cur->child_;
        }
        return {*this,cur,[](const Node_t*& cur){cur = cur->child_;}};
    }

    // O(1)
    LinkListIter<Node_t> rbegin() noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        return {*this,last_,[](Node_t*& cur){cur = cur->father_;}};
    }

    // O(1)
    LinkListIter<Node_t> rend() noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        return {*this,nullptr,[](Node_t*& cur){cur = cur->father_;}};
    }

    // O(1)
    LinkListIterConst<Node_t> rbegin() const noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        return {*this,last_,[](const Node_t*& cur){cur = cur->father_;}};
    }

    // O(1)
    LinkListIterConst<Node_t> rend() const noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        return {*this,nullptr,[](const Node_t*& cur){cur = cur->father_;}};
    }

    // O(n) !!!
    LinkListIter<Node_t> find(T const& _tar) noexcept
    requires(__is_eq_comparable<T>)
    {
        auto b = this->begin();
        auto e = this->end();
        while(b!=e){
            if constexpr(__has_opr_eq<T>){
                if(*b == _tar){
                    return b;
                }
            }
            else if constexpr(__has_opr_ineq<T>){
                if(!(*b!=_tar)){
                    return b;
                }
            }
            ++b;
        }
        return b;
    }

    // O(n) !!!
    LinkListIterConst<Node_t> find(T const& _tar) const noexcept
    requires(__is_eq_comparable<T>)
    {
        auto b = this->begin();
        auto e = this->end();
        while(b!=e){
            if constexpr(__has_opr_eq<T>){
                if(*b == _tar){
                    return b;
                }
            }
            else if constexpr(__has_opr_ineq<T>){
                if(!(*b!=_tar)){
                    return b;
                }
            }
            ++b;
        }
        return b;
    }

    // O(1)
    LinkListIter<Node_t> last() noexcept{
        return {*this,last_,[](Node_t*& cur){cur = cur->child_;}};
    }
    
    // O(1)
    LinkListIterConst<Node_t> last() const noexcept{
        return {*this,last_,[](const Node_t*& cur){cur = cur->child_;}};
    }

    // O(n) !!!
    T& operator[](usize _pos) {
        if constexpr (__is_SingleNode_t<Node_t>){
            // read from head
            Node_t* cur = root_;
            for(usize i = 0; i <_pos; i++){
                cur = cur->child_;
            }
            return *(cur->data_);
        }else {
            if(_pos >= (size_/2)){
                // read from tail
                Node_t* cur = last_;
                for(usize i = size_-1; i >_pos; i--){
                    cur = cur->father_;
                }
                return *(cur->data_);
            }else{
                // read from head
                Node_t* cur = root_;
                for(usize i = 0; i <_pos; i++){
                    cur = cur->child_;
                }
                return *(cur->data_);
            }

        }


    }
    
    // O(n) !!!
    T const& operator[](usize _pos)const {
        if constexpr (__is_SingleNode_t<Node_t>){
            // read from head
            const Node_t* cur = root_;
            for(usize i = 0; i <_pos; i++){
                cur = cur->child_;
            }
            return *(cur->data_);
        }else {
            if(_pos >= (size_/2)){
                // read from tail
                const Node_t* cur = last_;
                for(usize i = size_-1; i >_pos; i--){
                    cur = cur->father_;
                }
                return *(cur->data_);
            }else{
                // read from head
                const Node_t* cur = root_;
                for(usize i = 0; i <_pos; i++){
                    cur = cur->child_;
                }
                return *(cur->data_);
            }

        }
    }

    // O(1)
    usize size() const noexcept{
        return size_;
    }


// functions for Linklist operations

    // O(1)
    template<typename ...Args>
    void push_back(Args&& ... _data){
        __place_to_last(__alloc_node_with_data(std::forward<Args&&>(_data)...));
        return;
    }

    // O(1)
    template<typename ...Args>
    void push_front(Args&& ..._data){
        __place_to_front(__alloc_node_with_data(std::forward<Args&&>(_data)...));
    }
    
    // O(1)
    template<typename ...Args>
    void insert_back(LinkListIter<Node_t> const& _iter,Args&& ... _data) noexcept{
        __place_after(_iter,__alloc_node_with_data(std::forward<Args&&>(_data)...));
    }

    // O(1)
    template<typename ...Args>
    requires(__is_DoubleNode_t<Node_t>)
    void insert_front(LinkListIter<Node_t> const& _iter,Args&& ... _data) noexcept{
        __place_in_front_of(_iter,__alloc_node_with_data(std::forward<Args&&>(_data)...));
    }

    // O(1) : DoubleNode typed LinkList
    // O(n) : SingleNode typed LinkList  !!!
    T pop_back(){
        if(!size_){
            throw std::out_of_range{""};
        }else if(size_ == 1){
            
            if constexpr( std::is_move_constructible_v<T>){
                T res(std::move(*(this->root_->data_)));
                __free_single_node(this->root_);
                this->root_ = nullptr;
                this->last_ = nullptr;
                --size_;
                return std::move(res);
            }else{
                T res(*(this->root_->data_));
                __free_single_node(this->root_);
                this->root_ = nullptr;
                this->last_ = nullptr;
                --size_;
                return res;
            }

            (size_)--;
        }

        else{ // size_ >=2
            
            auto before_last = __find_node_before_last();


            if constexpr( std::is_move_constructible_v<T>){
                T res(std::move(*(last_->data_)));
                __free_single_node(last_);
                before_last->child_ = nullptr;
                this->last_ = before_last;
                --size_;
                return std::move(res);
            }else{
                T res(*(last_->data_));
                __free_single_node(last_);
                before_last->child_ = nullptr;
                this->last_ = before_last;
                --size_;
                return res;
            }

            

        }

    }

    // O(1)
    T pop_front(){
        if(!size_){
            throw std::out_of_range{""};
        }else{
            --this->size_;
            if constexpr( std::is_move_assignable_v<T>){
                T res(std::move(*(this->root_->data_)));

                Node_t* pre_root = this->root_;
                bool change_last = __is_last(pre_root);
                this->root_ = this->root_->child_;
                __free_single_node(pre_root);
                if constexpr(__is_DoubleNode_t<Node_t>){
                    this->root_->father_  = nullptr;
                }
                if(change_last){
                    this->last_ = nullptr;
                }
                return std::move(res);
            }else{
                T res(*(this->root_->data_));

                Node_t* pre_root = this->root_;
                bool change_last = __is_last(pre_root);
                this->root_ = this->root_->child_;
                __free_single_node(pre_root);
                if constexpr(__is_DoubleNode_t<Node_t>){
                    this->root_->father_  = nullptr;
                } 
                if(change_last){
                    this->last_ = nullptr;
                }
                return res;
            }
        }
    }

    // O(n) !!!
    // DoubleNode typed Linklist will be faster
    void reverse() noexcept {
        if constexpr(__is_DoubleNode_t<Node_t>){
            this->last_ = this->root_;
            Node_t* cur = this->root_;

            while(cur){
                auto next = cur->child_;

                auto temp = cur->father_;
                cur->father_ = cur->child_;
                cur->child_ = temp;

                if(!next){
                    this->root_ = cur;
                    return;
                }
                cur = next;

            }
            return;
        }else{
            if(!this->size_)return;
            LinkList<Node_t> temp = this->move();
            Node_t* cur = temp.root_;
            while (cur)
            {
                Node_t* next = cur->child_;
                this->__place_to_front(cur);
                cur = next;
            }
            temp.__leak();
        }

    }

    // O(1)
    void merge(LinkList<Node_t>&& _other) {
        if(!this->size_){
            *this = _other.move();
            return;
        }
        if(!_other.size_){
            return;
        }
        Node_t* other_root = _other.root_;

        last_->child_ = other_root;
        if constexpr (__is_DoubleNode_t<Node_t>){
            other_root->father_ = last_;
        }
        last_ = _other.last_;
        this->size_ += _other.size_;

        _other.size_ = 0;
        _other.root_ = nullptr;

    }

// the following functions will automatically move the iter to next
// be caution with these functions

    // O(1)
    // this function will automatically move the iter to next
    template<typename IterType>
    bool erase(IterType&& _iter) noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        if constexpr(!std::is_same_v<std::decay_t<IterType>,LinkListIter<Node_t>>){
            static_assert(false,"IterType must be rvref or lvref of LinkListIter<Node_t> ");
        }

        auto cur = _iter.cur_;
        if(!cur) return false;
        if(this->size_ == 0)return false;
        __free_single_node(__pick_a_node_out(_iter));
        return true;
    }

    // O(1)
    // this function is designed to move one node from this to other Linklist
    // it will completely move a single node with out alloc any space
    // after the node been successfully moved, this func will move "from_this" iter to next
    template<typename IterType_1 , typename IterType_2>
    void give_to_front(IterType_1&& from_this, IterType_2&& to_other)
    requires(__is_DoubleNode_t<Node_t>)
    {
        if constexpr(
            !std::is_same_v<std::decay_t<IterType_1>,LinkListIter<Node_t>>
            || 
            !std::is_same_v<std::decay_t<IterType_1>,std::decay_t<IterType_2>>){
            static_assert(false,"IterType_1 & IterType_2 must be rvref or lvref of LinkListIter<Node_t>");
        }

        to_other.owner_.__place_in_front_of(to_other,__pick_a_node_out(from_this));
    }

    // O(1)
    // this function is designed to move one node from this to other Linklist
    // it will completely move a single node with out alloc any space
    // after the node been successfully moved, this func will move "from_this" iter to next
    template<typename IterType_1 , typename IterType_2>
    void give_to_back(IterType_1&& from_this, IterType_2&& to_other)
    requires(__is_DoubleNode_t<Node_t>)
    {
        if constexpr(
            !std::is_same_v<std::decay_t<IterType_1>,LinkListIter<Node_t>>
            || 
            !std::is_same_v<std::decay_t<IterType_1>,std::decay_t<IterType_2>>){
            static_assert(false,"IterType_1 & IterType_2 must be rvref or lvref of LinkListIter<Node_t>");
        }

        to_other.owner_.__place_after(to_other,__pick_a_node_out(from_this));
    }

    // swap the data of two iters
    template<typename IterType_1 , typename IterType_2>
    static void swap(IterType_1&& _1, IterType_2&& _2)
    noexcept
    {
        if constexpr(
            !std::is_same_v<std::decay_t<IterType_1>,LinkListIter<Node_t>>
            || 
            !std::is_same_v<std::decay_t<IterType_1>,std::decay_t<IterType_2>>){
            static_assert(false,"IterType_1 & IterType_2 must be rvref or lvref of LinkListIter<Node_t>");
        }
        auto temp = _1.cur_->data_;
        _1.cur_->data_ = _2.cur_->data_;
        _2.cur_->data_ = temp;
    }

protected:
    // O(1)
    // just place the node to the last of the linklist
    void __place_to_last(Node_t* new_node) {
        ++this->size_;
        if(!last_){// no root
            this->root_ = new_node;
            if constexpr(__is_DoubleNode_t<Node_t>){
                new_node->father_ = nullptr;
            }
            last_ = new_node;
        }else{
            last_->child_ = new_node;
            if constexpr(__is_DoubleNode_t<Node_t>){
                new_node->father_ = last_;
            }
            last_ = last_->child_;
        }
        return;
    }
    
    // O(1)
    // just place the node to the front of the linklist
    void __place_to_front(Node_t* new_node){
        ++this->size_;
        bool modify_last_ = this->root_ == nullptr;
        new_node->child_ = this->root_;
        this->root_ = new_node;
        if constexpr(__is_DoubleNode_t<Node_t>){
            new_node->father_ = nullptr;
            if(new_node->child_){
                new_node->child_->father_ = new_node;
            }
        }
        if(modify_last_) last_ = new_node;
    }

    // O(1)
    // place the node after an iter
    void __place_after(LinkListIter<Node_t> const& _iter, Node_t* new_node) noexcept{
        auto cur = _iter.cur_;
        if(this->size_ == 0){
            __place_to_last(new_node);
        }else{
            ++this->size_;
            auto pre_child = cur->child_;
            bool modify_last = cur->child_ == nullptr;
            cur->child_ = new_node;
            cur->child_->child_ = pre_child;
            if constexpr (__is_DoubleNode_t<Node_t>){
                cur->child_->father_ = cur;
                if(pre_child) pre_child->father_ = cur->child_;
            }
            if(modify_last) last_ = cur->child_;
        }
    }
    
    // O(1)
    // place the node in front of an iter
    void __place_in_front_of(LinkListIter<Node_t> const& _iter, Node_t* new_node) noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        auto cur = _iter.cur_;
        if(this->size_ == 0){
            __place_to_front(new_node);
        }else{
            ++this->size_;
            auto pre_father = cur->father_;
            bool modify_root = pre_father==nullptr;
            cur->father_ = new_node;
            cur->father_->father_ = pre_father;
            cur->father_->child_ = cur;
            if(pre_father) pre_father->child_ = cur->father_;
            if(modify_root) this->root_ = cur->father_;
        }
    }

    // O(1)
    // this function will move the _iter to next
    Node_t* __pick_a_node_out(LinkListIter<Node_t> & _iter)noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        auto node = _iter.cur_;
        ++_iter;
        if(this->size_ == 1){
            auto res = this->root_;
            __remove_node_link(res);
            __leak();
            return res;
        }
        if(__is_root(node)){
            this->root_ = node->child_;
            this->root_->father_ = nullptr;
        }
        else if (__is_last(node)){
            this->last_ = node->father_;
            this->last_->child_ = nullptr;
        }else{
            node->father_->child_ = node->child_;
            node->child_->father_ = node->father_;
        }
        __remove_node_link(node);
        --this->size_;
        return node;
    }

    // O(1)
    // check if the node is the root node
    static bool __is_root(const Node_t * _node) noexcept
    requires(__is_DoubleNode_t<Node_t>)
    {
        return _node->father_ == nullptr;
    }
    
    // O(1)
    // check if the node is the last node
    static bool __is_last(const Node_t * _node) noexcept
    {
        return _node->child_ == nullptr;
    }

    // O(1)
    // free a node and its data without free its children
    static void __free_single_node(Node_t* _node) noexcept{
        if(_node){
            if(_node->data_){
                delete _node->data_;
            }
            delete _node;
        }
    }
    
    // O(1)
    // remove the father and child from a node
    static void __remove_node_link(Node_t* _node) noexcept{
        if constexpr(__is_DoubleNode_t<Node_t>){
            _node->father_ = nullptr;
        }
        _node->child_ = nullptr;
    }

    // O(n) !!! depends on the count of children
    // free a node and all of its children
    static void __free_node_and_child(Node_t* _node)noexcept {
        if(_node){
            if(_node->child_){
                __free_node_and_child(_node->child_);
            }
            delete _node->data_;
            delete _node;
        }
    }
    
    // O(1)
    // leak the link list, set the root and last to nullptr and set the size to zero
    // make sure this linklist had been move to another object before call this function
    void __leak() noexcept{
        this->root_ = nullptr;
        this->size_ = 0;
        this->last_ = nullptr;
    }

    // O(1)
    // create a node using standard {} constructors
    template<typename ...Args>
    static Node_t* __alloc_node_with_data(Args&&...args) noexcept{
        Node_t* new_node = new Node_t{};
        new_node->data_ = new T{std::forward<Args&&>(args)...};
        return new_node;
    }

    // O(1) : DoubleNode typed LinkList
    // O(n) : SingleNode typed LinkList  !!!
    // find the node before last node
    // the time complexity is O(n) with SingleNode typed Linklist
    // while it is O(1) with DoubleNode typed Linklist
    Node_t* __find_node_before_last(){
        if(size_ < 2){
            throw std::out_of_range{""};
        }

        if constexpr (__is_DoubleNode_t<Node_t>){
            return last_->father_;
        }

        Node_t* cur = root_;
        loop:
        if(cur->child_->child_){
            cur = cur->child_;
            goto loop;
        }else{
            return cur;
        }
    }


    Node_t* root_;
    usize size_;
    Node_t* last_;
};

} // namespace tcx


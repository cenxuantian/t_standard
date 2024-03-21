#pragma once
#include "../algorithm/t_math.hpp"
#include "../basic/t_class_helper.h"
#include "./t_tree_node.hpp"
#include "./t_tree_iter.hpp"
#include "../basic/t_meta.h"
#include <iostream>
#include <functional>

namespace tcx{

// pre def
template<typename Node_t>
requires(__is_comparable_v<typename Node_t::Data_t>)
class SearchBinaryTree :public NonCopyble {
// left smaller right bigger
public:
    using T = Node_t::Data_t;

    SearchBinaryTree(){}

    ~SearchBinaryTree(){
        __free_node_and_children(root_);
        size_ = 0;
    }

    SearchBinaryTree(SearchBinaryTree&& other)
    noexcept
    :root_(other.root_)
    ,size_(other.size_)
    {
        other.root_ = nullptr;
        other.min_ = nullptr;
        other.max_ = nullptr;
        other.size_ = 0;
    }

    SearchBinaryTree& operator=(SearchBinaryTree&& other)noexcept{
        __free_node_and_children(root_);
        this->root_  = other.root_;
        this->min_ =   other.min_;
        this->max_ =   other.max_;
        this->size_ =  other.size_;
        other.root_ = nullptr;
        other.min_ = nullptr;
        other.max_ = nullptr;
        other.size_ = 0;
    }

    SearchBinaryTree&& move()noexcept{
        return std::move(*this);
    }

    SearchBinaryTree copy() noexcept{
        SearchBinaryTree res;
        this->root_traverse([&res](T const& i){
            T each{i};
            res.emplace(std::move(each));
        });
        return res.move();
    }

    SearchBinaryTreeIter<Node_t> begin() noexcept{
        return {root_,__co_LSRP_traverse<Node_t,true,true,true>};
    }

    SearchBinaryTreeIter<Node_t> end() noexcept{
        return {nullptr,__co_LSRP_traverse<Node_t,true,true,true>};
    }

    SearchBinaryTreeConstIter<Node_t> begin() const noexcept{
        return {root_,__co_LSRP_traverse<Node_t,true,true,true>};
    }

    SearchBinaryTreeConstIter<Node_t> end()  const noexcept{
        return {nullptr,__co_LSRP_traverse<Node_t,true,true,true>};
    }

    SearchBinaryTreeIter<Node_t> rbegin() noexcept{
        return {root_,__co_RSLP_traverse<Node_t,true,true,true>};
    }

    SearchBinaryTreeIter<Node_t> rend() noexcept{
        return {nullptr,__co_RSLP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<Node_t> rbegin() const noexcept{
        return {root_,__co_RSLP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<Node_t> rend()  const noexcept{
        return {nullptr,__co_RSLP_traverse<Node_t,true,true,true>};
    }

    const T& min() const noexcept{
        auto cur_left = root_;
        while(cur_left->lchild){
            cur_left = cur_left->lchild;
        }
        return *(cur_left->data);
    }

    const T& max() const noexcept{
        auto cur_right = root_;
        while(cur_right->rchild){
            cur_right = cur_right->rchild;
        }
        return *(cur_right->data);
    }

    void inc_traverse(std::function<void(T&)> const& func) noexcept{
        __LSRP_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    void inc_traverse(std::function<void(T const&)> const& func) const noexcept{
        __LSRP_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    void dec_traverse(std::function<void(T&)> const& func)noexcept{
        __RSLP_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    void dec_traverse(std::function<void(T const&)> const& func) const noexcept{
        __RSLP_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    void root_traverse(std::function<void(T&)> const& func)noexcept{
        __PSLR_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    void root_traverse(std::function<void(T const&)> const& func) const noexcept{
        __PSLR_traverse<T,true,true,true>(root_,[&func](Node_t* _node){
            func(*(_node->data));
        });
    }

    template<typename ...Args>
    void emplace(Args&&... args) noexcept{
        auto new_node = __alloc_new_node(std::forward<Args&&>(args)...);
        if(empty()){
            root_ = new_node;
        }else{
            __add_new_node(new_node);
        }
        ++size_;
    }

    SearchBinaryTreeConstIter<Node_t> find(__Faster_func_param_t<T> item) const noexcept{
        auto cur = root_;
        loop:
        if(cur){
            int comp_res = compare<__Faster_func_param_t<T>>(*(cur->data),item);
            if(comp_res == 0){
                return {cur,__co_PSLR_traverse<T,true,true,false>};
            }else if(comp_res>0){
                if(cur->lchild){
                    cur = cur->lchild;
                    goto loop;
                }else{
                    return {nullptr,__co_PSLR_traverse<T,false,false,false>};
                }
            }else{
                if(cur->rchild){
                    cur = cur->rchild;
                    goto loop;
                }else{
                    return {nullptr,__co_PSLR_traverse<T,false,false,false>};
                }
            }
        }
        else return {nullptr,__co_PSLR_traverse<T,false,false,false>};
    }

    template<typename IterType>
    void erase(IterType&& iter){
        if constexpr(!__belongs_to_v<std::decay_t<IterType>(SearchBinaryTreeConstIter<Node_t>,SearchBinaryTreeIter<Node_t>)>){
            static_assert(false,"IterType must be SearchBinaryTreeConstIter<Node_t> or SearchBinaryTreeIter<Node_t>");
        }
        auto node = iter.cur_;
        iter.cur_ = nullptr;    // set iter to end
        if(node){   // iter is not end
            __take_node_out(node);
            __free_single_node(const_cast<Node_t*&>(node));
        }
    }

    usize size() const noexcept{
        return this->size_;
    }

    bool empty()const noexcept{
        return size_==0;
    }

protected:
    // exchange the data of two nodes
    inline static void __exchange_data(Node_t* _1,Node_t* _2)noexcept{
        auto temp = _1->data;
        _1->data = _2->data;
        _2->data = _1->data;
        return;
    }

    // node could not be a nullptr
    inline static bool __is_a_lchild(const Node_t* node)noexcept{
        return node == node->parent->lchild;
    }
    
    // node could not be a nullptr
    inline static bool __is_a_rchild(const Node_t* node)noexcept{
        return node == node->parent->rchild;
    }

    // get the uncle node of a node
    // returns nullptr if node do not have an uncle
    inline static Node_t* __get_uncle(const Node_t* node) noexcept{
        if(node->parent){
            Node_t* grad_parent = node->parent->parent;
            if(grad_parent){
                if(grad_parent->lchild == node->parent){
                    return grad_parent->rchild;
                }else{
                    return grad_parent->lchild;
                }
            }
        }
        return nullptr;
    }

    // get the sibling node of a node
    // returns nullptr if node do not have an sibling
    inline static Node_t* __get_sibling(const Node_t* node)noexcept{
        if(!node->parent) return nullptr;
        if(__is_a_lchild(node)) return node->rchild;
        else return node->lchild;
    }

    // check if the node is a root node
    inline static bool __is_root(const Node_t* node)noexcept{
        return node->parent == nullptr;
    }

    inline static u8 __count_children(const Node_t* node) noexcept{
        u8 res = 0;
        if(node->lchild) ++res;
        if(node->rchild) ++res;
        return res;
    }

    // allocate new node
    // returns new node
    template<typename ...Args>
    inline static Node_t* __alloc_new_node(Args&& ...args)noexcept{
        return new Node_t{
            new Node_t::Data_t{std::forward<Args&&>(args)...},
            nullptr,nullptr,nullptr};
    }

    // 
    inline static void __free_single_node(Node_t*& node) noexcept {
        if(node){
            node->lchild = nullptr;
            node->rchild = nullptr;
            node->parent = nullptr;
            delete node->data;
            node->data = nullptr;
            delete node;
            node = nullptr;
        }
    }

    inline static void __free_node_and_children(Node_t*& node) noexcept {
        if(node){
            delete node->data;
            node->data = nullptr;
            __free_node_and_children(node->lchild);
            __free_node_and_children(node->rchild);
            delete node;
            node = nullptr;
        }
    }

    virtual inline void __lrotate(Node_t* node) noexcept{
        Node_t* origi_p = node->parent;
        if(origi_p){
            if(__is_a_lchild(node)){
                origi_p->lchild = node->rchild;
                origi_p->lchild->parent = origi_p;
                node->rchild = origi_p->lchild->lchild;
                node->parent =origi_p->lchild;
                origi_p->lchild->lchild = node;
            }else{
                origi_p->rchild = node->rchild;
                origi_p->rchild->parent = origi_p;
                node->rchild = origi_p->rchild->lchild;
                node->parent =origi_p->rchild;
                origi_p->rchild->lchild = node;
            }

        }else{
            this->root_ = node->rchild;
            this->root_->parent = nullptr;
            node->parent = this->root_;
            node->rchild = this->root_->lchild;
            this->root_->lchild = node;
        }
    }

    virtual inline void __rrotate(Node_t* node) noexcept{
        Node_t* origi_p = node->parent;
        if(origi_p){
            if(__is_a_lchild(node)){
                origi_p->lchild = node->lchild;
                origi_p->lchild->parent = origi_p;
                node->lchild = origi_p->lchild->rchild;
                node->parent =origi_p->lchild;
                origi_p->lchild->rchild = node;
            }else{
                origi_p->rchild = node->lchild;
                origi_p->rchild->parent = origi_p;
                node->lchild = origi_p->rchild->rchild;
                node->parent =origi_p->rchild;
                origi_p->rchild->rchild = node;
            }
        }else{
            this->root_ = node->lchild;
            this->root_->parent = nullptr;
            node->parent = this->root_;
            node->lchild = this->root_->rchild;
            this->root_->rchild = node;
        }
    }
    
    // add a new node into suitable position
    void __add_new_node(Node_t* new_node) noexcept{
        Node_t* cur_comp = this->root_;
        while (1)
        {
            if(compare<__Faster_func_param_t<decltype(*(cur_comp->data))>>(*(cur_comp->data),*(new_node->data)) >=0){
                // move to left
                if(cur_comp->lchild){
                    cur_comp = cur_comp->lchild;
                    continue;
                }else{
                    cur_comp->lchild = new_node;
                    new_node->parent = cur_comp;
                    break;
                }
            }else{
                // move to right
                if(cur_comp->rchild){
                    cur_comp = cur_comp->rchild;
                    continue;
                }else{
                    cur_comp->rchild = new_node;
                    new_node->parent = cur_comp;
                    break;
                }
            }
        }
    }

    // take a node out
    void __take_node_out(Node_t* node){
        auto p = node->parent;
        auto l = node->lchild;
        auto r = node->rchild;
        if(p){
            if(p->lchild == node){
                if(l){
                    p->lchild = l;
                    l->parent = p;
                    auto _cur_right = l;
                    while(_cur_right->rchild){
                        _cur_right = _cur_right->rchild;
                    }
                    _cur_right->rchild = r;
                    if(r) r->parent = _cur_right;
                }
                else if(r){
                    p->lchild = r;
                    r->parent = p;
                    if(this->min_ == node){
                        auto _cur_left = r;
                        while(_cur_left->lchild){
                            _cur_left = _cur_left->lchild;
                        }
                        this->min_ = _cur_left;
                    }
                }else{
                    p->lchild = nullptr;
                    if(this->min_ == node){
                        this->min_ = p;
                    }else if(this->max_ == node){
                        this->max_ = p;
                    }
                }

            }else{
                if(l){
                    p->rchild = l;
                    l->parent = p;
                    auto _cur_right = l;
                    while(_cur_right->rchild){
                        _cur_right = _cur_right->rchild;
                    }
                    _cur_right->rchild = r;
                    if(r) r->parent = _cur_right;
                }
                else if(r){
                    p->rchild = r;
                    r->parent = p;
                    if(this->max_ == node){
                        auto _cur_right = r;
                        while(_cur_right->rchild){
                            _cur_right = _cur_right->lchild;
                        }
                        this->max_ = _cur_right;
                    }
                }else{
                    p->rchild = nullptr;
                    if(this->min_ == node){
                        this->min_ = p;
                    }else if(this->max_ == node){
                        this->max_ = p;
                    }
                }

            }
        }else{
            // is root
            if(l){  // min_ do not need to be changed
                this->root_ = l;
                this->root_->parent = nullptr;
                auto _cur_right = root_;
                while(_cur_right->rchild){
                    _cur_right = _cur_right->rchild;
                }
                _cur_right->rchild = r;
                if(this->max_ == node){ // node do not have rchild
                    this->max_ = _cur_right;
                }else{  // r != nullptr
                    r->parent = _cur_right;
                }
            }else if(r){    // max_ do not need to be changed
                this->root_ = r;
                this->root_->parent = nullptr;
                this->min_ = r;
            }else{
                this->root_ = nullptr;
                this->min_ = nullptr;
                this->max_ = nullptr;
            }
        }
    
        --this->size_;
    }

    Node_t* root_ = nullptr;
    usize size_ = 0;
};

}
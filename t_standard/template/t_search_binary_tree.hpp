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
template<typename T>
requires(__is_comparable_v<T>)
class SearchBinaryTree :public NonCopyble {
// left smaller right bigger
public:

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

    SearchBinaryTreeIter<T> begin() noexcept{
        return {root_,__co_LSRP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeIter<T> end() noexcept{
        return {nullptr,__co_LSRP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<T> begin() const noexcept{
        return {root_,__co_LSRP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<T> end()  const noexcept{
        return {nullptr,__co_LSRP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeIter<T> rbegin() noexcept{
        return {root_,__co_RSLP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeIter<T> rend() noexcept{
        return {nullptr,__co_RSLP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<T> rbegin() const noexcept{
        return {root_,__co_RSLP_traverse<T,true,true,true>};
    }

    SearchBinaryTreeConstIter<T> rend()  const noexcept{
        return {nullptr,__co_RSLP_traverse<T,true,true,true>};
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
        __LSRP_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void inc_traverse(std::function<void(T const&)> const& func) const noexcept{
        __LSRP_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void dec_traverse(std::function<void(T&)> const& func)noexcept{
        __RSLP_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void dec_traverse(std::function<void(T const&)> const& func) const noexcept{
        __RSLP_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void root_traverse(std::function<void(T&)> const& func)noexcept{
        __PSLR_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void root_traverse(std::function<void(T const&)> const& func) const noexcept{
        __PSLR_traverse<T,true,true,true>(root_,[&func](BinaryTreeNode<T>* _node){
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

    SearchBinaryTreeConstIter<T> find(__Faster_func_param_t<T> item) const noexcept{
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
        if constexpr(!__belongs_to_v<std::decay_t<IterType>(SearchBinaryTreeConstIter<T>,SearchBinaryTreeIter<T>)>){
            static_assert(false,"IterType must be SearchBinaryTreeConstIter<T> or SearchBinaryTreeIter<T>");
        }
        auto node = iter.cur_;
        iter.cur_ = nullptr;    // set iter to end
        if(node){   // iter is not end
            __take_node_out(node);
            __free_single_node(const_cast<BinaryTreeNode<T>*&>(node));
            --this->size_;
        }
    }

    usize size() const noexcept{
        return this->size_;
    }

    bool empty()const noexcept{
        return size_==0;
    }

protected:

    void __add_new_node(BinaryTreeNode<T>* new_node){
        BinaryTreeNode<T>* cur_comp = root_;
        while (1)
        {
            if(compare<__Faster_func_param_t<T>>(*cur_comp->data,*new_node->data) >=0){
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

    static void __free_single_node(BinaryTreeNode<T>*& node) noexcept {
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

    static void __free_node_and_children(BinaryTreeNode<T>*& node) noexcept {
        if(node){
            delete node->data;
            node->data = nullptr;
            __free_node_and_children(node->lchild);
            __free_node_and_children(node->rchild);
            delete node;
            node = nullptr;
        }
    }

    void __take_node_out(BinaryTreeNode<T>* node){
        auto p = node->parent;
        auto l = node->lchild;
        auto r = node->rchild;
        if(p){
            if(p->lchild == node){  // max_ do not need to be changed
                if(l){  // min_ & max_ do not need to be changed
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
                    if(this->min_ == node){ // change min_
                        auto _cur_left = r;
                        while(_cur_left->lchild){
                            _cur_left = _cur_left->lchild;
                        }
                        this->min_ = _cur_left;
                    }
                }else{
                    p->lchild = nullptr;
                    if(this->min_ == node){ // change max_ | min_
                        this->min_ = p;
                    }else if(this->max_ == node){
                        this->max_ = p;
                    }
                }

            }else{  // min_ do not need to be changed
                if(l){  // min_ & max_ do not need to be changed
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
                    if(this->max_ == node){ // change max_
                        auto _cur_right = r;
                        while(_cur_right->rchild){
                            _cur_right = _cur_right->lchild;
                        }
                        this->max_ = _cur_right;
                    }
                }else{
                    p->rchild = nullptr;
                    if(this->min_ == node){ // change max_ | min_
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
    }

    template<typename ...Args>
    static BinaryTreeNode<T>* __alloc_new_node(Args&& ...args)noexcept{
        return new BinaryTreeNode<T>{
            new T{std::forward<Args&&>(args)...},
            nullptr,nullptr,nullptr};
    }

    BinaryTreeNode<T>* root_ = nullptr;
    usize size_ = 0;
};

}
#pragma once
#include "../algorithm/t_math.hpp"
#include "../basic/t_class_helper.h"
#include <iostream>
#include <functional>

namespace tcx{
using usize = unsigned long long;

template<typename T>
struct BinaryTreeNode{
    T* data;
    BinaryTreeNode* lchild;
    BinaryTreeNode* rchild;
    BinaryTreeNode* parent;
};

template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent, bool _traverse_self>
void __post_traverse(BinaryTreeNode<T>* node, std::function<void(BinaryTreeNode<T>*)> const& func){
    if(node){
        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                func(node->rchild);
                __post_traverse<T,true,true,false,false>(node->rchild,func);
            }
        }

        
        // self
        if constexpr(_traverse_self){
            func(node);
        }

        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                __post_traverse<T,true,true,false,true>(node->lchild,func);
            }
        }

        // parent
        if constexpr (_traverse_parent){
            __post_traverse<T,true,false,true,true>(node->parent,func);
        }
    }
}

template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent, bool _traverse_self>
void __mid_traverse(BinaryTreeNode<T>* node, std::function<void(BinaryTreeNode<T>*)> const& func){
    if(node){
        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                func(node->lchild);
                __mid_traverse<T,true,true,false,false>(node->lchild,func);
            }
        }

        // self
        if constexpr(_traverse_self){
            func(node);
        }
        
        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                __mid_traverse<T,true,true,false,true>(node->rchild,func);
            }
        }

        // parent
        if constexpr (_traverse_parent){
            __mid_traverse<T,false,true,true,true>(node->parent,func);
        }
    }
}

template<typename T>
// reqiure_compareble
class SearchBinaryTree :public NonCopyble {
// left smaller right bigger
public:

    SearchBinaryTree(){}

    ~SearchBinaryTree(){
        __free_node_and_children(first_);
        min_ = nullptr;
        max_ = nullptr;
        size_ = 0;
    }

    SearchBinaryTree(SearchBinaryTree&& other)
    noexcept
    :first_(other.first_)
    ,min_(other.min_)
    ,max_(other.max_)
    ,size_(other.size_)
    {
        other.first_ = nullptr;
        other.min_ = nullptr;
        other.max_ = nullptr;
        other.size_ = 0;
    }

    SearchBinaryTree& operator=(SearchBinaryTree&& other)noexcept{
        __free_node_and_children(first_);
        this->first_  = other.first_;
        this->min_ =   other.min_;
        this->max_ =   other.max_;
        this->size_ =  other.size_;
        other.first_ = nullptr;
        other.min_ = nullptr;
        other.max_ = nullptr;
        other.size_ = 0;
    }

    SearchBinaryTree&& move()noexcept{
        return std::move(*this);
    }

    SearchBinaryTree copy() noexcept{
        SearchBinaryTree res;
        this->post_traverse([&res](T const& i){
            T each{i};
            res.emplace(std::move(each));
        });
        return res.move();
    }

    template<typename ...Args>
    void emplace(Args&&... args){
        auto new_node = __alloc_new_node(std::forward<Args&&>(args)...);
        if(__empty()){
            first_ = new_node;
            min_ = first_;
            max_ = first_;
        }else{
            BinaryTreeNode<T>* cur_comp = first_;
            bool left_changed = false;
            bool right_changed = false;
            while (1)
            {
                if(compare(*cur_comp->data,*new_node->data) >=0){
                    // move to left
                    if(!left_changed)left_changed=true;
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
                    if(!right_changed)right_changed=true;
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
            
            if(left_changed && !right_changed){
                min_ = new_node;
            }else if (!left_changed && right_changed){
                max_ = new_node;
            }

        }
        ++size_;
    }

    const T& min() const noexcept{
        return *(min_->data);
    }

    const T& max() const noexcept{
        return *(max_->data);
    }

    void mid_traverse(std::function<void(T&)> const& func){
        __mid_traverse<T,true,true,true,true>(min_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void mid_traverse(std::function<void(T const&)> const& func) const{
        __mid_traverse<T,true,true,true,true>(min_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void post_traverse(std::function<void(T&)> const& func){
        __post_traverse<T,true,true,true,true>(max_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

    void post_traverse(std::function<void(T const&)> const& func) const{
        __post_traverse<T,true,true,true,true>(max_,[&func](BinaryTreeNode<T>* _node){
            func(*(_node->data));
        });
    }

private:

    bool __empty() const noexcept{
        return size_==0;
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

    template<typename ...Args>
    static BinaryTreeNode<T>* __alloc_new_node(Args&& ...args)noexcept{
        return new BinaryTreeNode<T>{
            new T{std::forward<Args&&>(args)...},
            nullptr,nullptr,nullptr};
    }

    BinaryTreeNode<T>* first_ = nullptr;
    BinaryTreeNode<T>* min_ = nullptr;
    BinaryTreeNode<T>* max_ = nullptr;
    usize size_ = 0;
};

}
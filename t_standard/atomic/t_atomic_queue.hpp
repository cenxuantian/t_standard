#pragma once

#include <atomic>
#include "../basic/t_class_helper.h"
#include "t_atomic_node.hpp"
namespace tcx
{
    
template<typename T>
class AtomicQueue :public NonCopyble{
public:
    using Node_t = AtomicNode<T>;

    AtomicQueue():root_(nullptr){}
    AtomicQueue(AtomicQueue&& _other)noexcept :root_(nullptr) {
        Node_t* other_root = _other.root_.load();
        while(!_other.root_.compare_exchange_strong(other_root,nullptr)){
            other_root = _other.root_.load();
            goto loop;
        }
        this->root_.store(other_root);
    }

    AtomicQueue& operator=(AtomicQueue&& _other) = delete;

    template<typename ...Args>
    void push(Args&& ...args){
        Node_t* new_node = new Node_t();
        Node_t* new_root = new Node_t();
        new_node->data_ = new T(std::forward<Args&&>(args)...);


        // set new node
        Node_t* cur_root = this->root_.load();
        new_node->root_ = cur_root;
        if(!new_node->root_) {
            new_node->root_ = new_node;
            new_node->last_ = new_node;
        }else{
            new_node->last_ = cur_root->last_;
        }

        if(cur_root){
            
        }
        

    }

private:



    volatile std::atomic<Node_t*> root_;



};

} // namespace tcx

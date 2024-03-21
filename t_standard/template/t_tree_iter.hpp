#pragma once

#include <functional>
#include "../basic/t_coroutine.hpp"
#include "t_node.hpp"

namespace tcx
{

template<typename T>
class SearchBinaryTreeIter :public NonCopyble,NonMoveable{
public:
    SearchBinaryTreeIter(BinaryTreeNode<T>* start, OnlyFinalSusCoro<T>(*coro_ini_func)(BinaryTreeNode<T>*, void*))
    noexcept :cur_(nullptr),co_(coro_ini_func(start,this)){};

    SearchBinaryTreeIter<T>& operator++(){
        co_.handle.resume();
        return *this;
    };
    auto& operator*()  {
        return *(cur_->data);
    };
    bool operator!=(SearchBinaryTreeIter<T> const& _other) {
        return _other.cur_ != cur_;
    }
    bool operator==(SearchBinaryTreeIter<T> const& _other) {
        return _other.cur_ == cur_;
    }
    BinaryTreeNode<T>* cur_;
private:
    OnlyFinalSusCoro<T> co_;


};

template<typename T>
class SearchBinaryTreeConstIter {
public:
    SearchBinaryTreeConstIter(const BinaryTreeNode<T>* start, OnlyFinalSusCoro<T>(*coro_ini_func)(BinaryTreeNode<T>*, void*))
    noexcept :cur_(nullptr),co_(coro_ini_func(const_cast<BinaryTreeNode<T>*>(start),this)){};

    SearchBinaryTreeConstIter<T>& operator++(){
        co_.handle.resume();
        return *this;
    };
    T const& operator*()  {
        return *(cur_->data);
    };
    bool operator!=(SearchBinaryTreeConstIter<T> const& _other) {
        return _other.cur_ != cur_;
    }
    bool operator==(SearchBinaryTreeConstIter<T> const& _other) {
        return _other.cur_ == cur_;
    }
    const BinaryTreeNode<T>* cur_;
private:
    OnlyFinalSusCoro<T> co_;
};

// normal traverse functions

// parent -> self -> left -> right
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
void __PSLR_traverse(BinaryTreeNode<T>* node, std::function<void(BinaryTreeNode<T>*)> const& func){
    if(node){
        // parent
        if constexpr (_traverse_parent){
            if(node->parent){
                if(node->parent->lchild == node){
                    __PSLR_traverse<T,false,true,true>(node->parent,func);
                }else{
                    __PSLR_traverse<T,true,false,true>(node->parent,func);
                }
            }
        }

        // self
        func(node);

        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                __PSLR_traverse<T,true,true,false>(node->lchild,func);
            }
        }

        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                __PSLR_traverse<T,true,true,false>(node->rchild,func);
            }
        }
    }
}

// right -> self -> left -> parent
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
void __RSLP_traverse(BinaryTreeNode<T>* node, std::function<void(BinaryTreeNode<T>*)> const& func){
    if(node){
        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                __RSLP_traverse<T,true,true,false>(node->rchild,func);
            }
        }

        // self
        func(node);

        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                __RSLP_traverse<T,true,true,false>(node->lchild,func);
            }
        }

        // parent
        if constexpr (_traverse_parent){
            __RSLP_traverse<T,true,false,true>(node->parent,func);
        }
    }
}

// left -> self -> right -> parent
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
void __LSRP_traverse(BinaryTreeNode<T>* node, std::function<void(BinaryTreeNode<T>*)> const& func){
    if(node){
        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                __LSRP_traverse<T,true,true,false>(node->lchild,func);
            }
        }
        // self
        func(node);
        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                __LSRP_traverse<T,true,true,false>(node->rchild,func);
            }
        }
        // parent
        if constexpr (_traverse_parent){
            __LSRP_traverse<T,false,true,true>(node->parent,func);
        }
    }
}


// coroutine based traverse functions

// left -> self -> right -> parent
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
OnlyFinalSusCoro<T> __co_LSRP_traverse(BinaryTreeNode<T>* node, void* _ctx){
    if(node){
        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                auto co = __co_LSRP_traverse<T,true,true,false>(node->lchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }
        // self
        ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = node;
        co_await std::suspend_always{};

        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                auto co = __co_LSRP_traverse<T,true,true,false>(node->rchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }
        // parent
        if constexpr (_traverse_parent){
            auto co = __co_LSRP_traverse<T,false,true,true>(node->parent,_ctx);
            while(!co.handle.done()){
                co_await std::suspend_always{};
                co.handle.resume();
            }
        }
    }
    
    ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = nullptr;
}

// right -> self -> left -> parent
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
OnlyFinalSusCoro<T> __co_RSLP_traverse(BinaryTreeNode<T>* node, void* _ctx){
    if(node){
        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                auto co = __co_RSLP_traverse<T,true,true,false>(node->rchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }

        // self
        ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = node;
        co_await std::suspend_always{};

        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                auto co = __co_RSLP_traverse<T,true,true,false>(node->lchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }

        // parent
        if constexpr (_traverse_parent){
            auto co = __co_RSLP_traverse<T,true,false,true>(node->parent,_ctx);
            while(!co.handle.done()){
                co_await std::suspend_always{};
                co.handle.resume();
            }
        }
    }
    
    ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = nullptr;
}

// parent -> self -> left -> right
template<typename T,bool _traverse_lchild, bool _traverse_rchild, bool _traverse_parent>
OnlyFinalSusCoro<T> __co_PSLR_traverse(BinaryTreeNode<T>* node, void* _ctx){
    if(node){
        // parent
        if constexpr (_traverse_parent){
            if(node->parent){
                if(node->parent->lchild == node){
                    auto co = __co_PSLR_traverse<T,false,true,true>(node->parent,_ctx);
                    while(!co.handle.done()){
                        co_await std::suspend_always{};
                        co.handle.resume();
                    }
                }else{
                    auto co = __co_PSLR_traverse<T,true,false,true>(node->parent,_ctx);
                    while(!co.handle.done()){
                        co_await std::suspend_always{};
                        co.handle.resume();
                    }
                }
            }
        }

        // self
        ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = node;
        co_await std::suspend_always{};

        // left
        if constexpr (_traverse_lchild){
            if(node->lchild){
                auto co = __co_PSLR_traverse<T,true,true,false>(node->lchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }

        // right
        if constexpr (_traverse_rchild){
            if(node->rchild){
                auto co = __co_PSLR_traverse<T,true,true,false>(node->rchild,_ctx);
                while(!co.handle.done()){
                    co_await std::suspend_always{};
                    co.handle.resume();
                }
            }
        }
    }
    ((SearchBinaryTreeIter<T>*)_ctx)->cur_ = nullptr;
}


} // namespace tcx


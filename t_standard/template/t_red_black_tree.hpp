#pragma once

#include "./t_search_binary_tree.hpp"

namespace tcx
{

enum class RedBlackTreeColor{
    BLACK = 0,
    RED = 1,
};

template<typename T>
struct RedBlackTreeItem{
    T data;
    RedBlackTreeColor color;
};

template<typename T>
requires(__is_comparable_v<T>)
class RedBlackTree : public SearchBinaryTree<RedBlackTreeItem<T>>{
public:

    using Node_t = BinaryTreeNode<RedBlackTreeItem<T>>;

    RedBlackTree():SearchBinaryTree<RedBlackTreeItem<T>>(){}

    RedBlackTree(RedBlackTree&& other):SearchBinaryTree<RedBlackTreeItem<T>>(std::forward<RedBlackTree&&>(other)){}

    template<typename ...Args>
    void emplace(Args&& ...args){
        // situation 1
        if(this->empty()){
            this->root_ = __alloc_new_red_black_node<RedBlackTreeColor::BLACK>(std::forward<Args&&>(args)...);
            ++this->size_;
            return;
        }
        

        Node_t* new_node = __alloc_new_red_black_node<RedBlackTreeColor::RED>(std::forward<Args&&>(args)...);
        Node_t* cur = this->root_;
        
        

        ++this->size_;
        return;
    }

private:
    inline static RedBlackTreeColor  __get_color(const Node_t* node) noexcept{
        return node->data->color;
    }
    inline static void  __set_color(Node_t* node, RedBlackTreeColor color) noexcept{
        node->data->color = color;
    }

    inline static Node_t* __get_uncle(Node_t* node) noexcept{
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

    static int __compare_node(Node_t* _1,Node_t* _2)noexcept{
        return compare<RedBlackTreeItem<T>const&>(_1->data->data,_1->data->data);
    }

    template<RedBlackTreeColor color ,typename ...Args>
    static Node_t* __alloc_new_red_black_node(Args&& ...args)noexcept {
        return new Node_t{
            new RedBlackTreeItem<T>{T{std::forward<Args&&>(args)...},color},
            nullptr,nullptr,nullptr};
    }
};


} // namespace tcx

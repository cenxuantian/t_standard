#pragma once

#include "./t_search_binary_tree.hpp"

namespace tcx
{



template<typename T, HowToDealWithDuplicate how_to_deal_with_duplicate = HowToDealWithDuplicate::TO_LEFT>
requires(__is_comparable_v<T>)
class RedBlackTree : public SearchBinaryTree<RedBlackTreeNode<T>,how_to_deal_with_duplicate>{
public:

    using Self_t = RedBlackTree<T>;
    using Father_t = SearchBinaryTree<RedBlackTreeNode<T>,how_to_deal_with_duplicate>;
    using Node_t = RedBlackTreeNode<T>;
    
    RedBlackTree():Father_t(){}

    RedBlackTree(RedBlackTree&& other):Father_t(std::forward<RedBlackTree&&>(other)){}

    template<typename ...Args>
    bool emplace(Args&& ...args) noexcept{
        // alloc new_node
        Node_t* new_node = Father_t::__alloc_new_node(std::forward<Args&&>(args)...);
        __set_color(new_node,RedBlackTreeColor::RED);

        if(this->empty()){
            // situation 1 : new node is root
            this->root_ = new_node;
            __set_color(new_node,RedBlackTreeColor::BLACK);
            ++this->size_;
            return false;
        }

        // add new_node to the tree
        if(!Father_t::__add_new_node(new_node)) return false;

        // new_node is not root_, it must got parent node

        Node_t* parent = new_node->parent;
        Node_t* uncle = Father_t::__get_uncle(new_node);

        if (__is_black(parent)){
            // situation 2
            ++this->size_;
            return true;
        }

        // parent is red currently, must have an uncle , so must got grandparent

        if(__is_red(uncle)){
            // situation 3
            __set_color(uncle,RedBlackTreeColor::BLACK);
            __set_color(parent,RedBlackTreeColor::BLACK);

            Node_t* cur = parent->parent;

            if(Father_t::__is_root(cur)){     // is root , root is always black
                ++this->size_;
                return true;
            }

            __set_color(cur,RedBlackTreeColor::RED);
            cur = cur->parent;

            loop:
            if(__is_black(cur)){
                ++this->size_;
                return true;
            }
            // cur is not root, cur is not black
            __set_color(cur,RedBlackTreeColor::BLACK);
            __set_color(cur->parent,RedBlackTreeColor::RED);
            cur = cur->parent;
            goto loop;
        }
        
        // situation 4  : father is red uncle is black or null
        if(Father_t::__is_a_lchild(parent)){
            if(Father_t::__is_a_lchild(new_node)){
                // ll
                __rrotate(parent->parent);
            }else{
                // lr
                __lrotate(parent);  // after this parent become
                __rrotate(new_node->parent);
            }
        }else{
            if(Father_t::__is_a_lchild(new_node)){
                // rl
                __rrotate(parent);
                __lrotate(new_node->parent);
            }else{
                // rr
                __lrotate(parent->parent);
            }
        }

        pos_end:
        ++this->size_;
        return true;
    }
    
    template<typename IterType>
    void erase(IterType&& iter) {
        if constexpr(!__belongs_to_v<std::decay_t<IterType>(SearchBinaryTreeConstIter<Node_t>,SearchBinaryTreeIter<Node_t>)>){
            static_assert(false,"IterType must be SearchBinaryTreeConstIter<Node_t> or SearchBinaryTreeIter<Node_t>");
        }
        auto node = iter.cur_;
        iter.cur_ = nullptr;    // set iter to end
        if(node){   // iter is not end
            Self_t::__take_node_out(const_cast<Node_t*&>(node));
            Father_t::__free_single_node(const_cast<Node_t*&>(node));
        }
    }

protected:

    inline static RedBlackTreeColor  __get_color(const Node_t* node) noexcept{
        return node->color;
    }
    
    inline static bool __is_red(const Node_t* node) noexcept{
        if(!node) return false;
        return node->color == RedBlackTreeColor::RED;
    }

    inline static bool __is_black(const Node_t* node) noexcept{
        if(!node) return true;
        return node->color == RedBlackTreeColor::BLACK;
    }

    inline static void  __set_color(Node_t* node, RedBlackTreeColor color) noexcept{
        node->color = color;
    }

    inline static void __exchange_color(Node_t* _1,Node_t* _2)noexcept{
        auto temp = __get_color(_1);
        __set_color(_1,__get_color(_2));
        __set_color(_2,temp);
        return;
    }

    virtual inline void __lrotate(Node_t* node) noexcept override{
        __exchange_color(node,node->rchild);
        this->Father_t::__lrotate(node);
    }

    virtual inline void __rrotate(Node_t* node) noexcept override{
        __exchange_color(node,node->lchild);
        this->Father_t::__rrotate(node);
    }

    void __take_node_out(Node_t*& node) noexcept{
        if(!node->lchild && !node->rchild){
            // node have no lchild and no rchild
            if(__is_red(node)){ // situation 1 : delete a red node
                // this is simple, just take this node out
                // and set the lchild or rchild value of its parent node to nullptr
                if(node->parent){
                    if(Father_t::__is_a_lchild(node)){
                        node->parent->lchild = nullptr;
                    }else{
                        node->parent->rchild = nullptr;
                    }
                    --this->size_;
                    return;
                }else{
                    this->root_ = nullptr;
                    --this->size_;
                    return;
                }
            }
            else{   // situation 2 : delete a black node
                    // this will break one of the ruls of red-black-tree
                    // so we need to adjust the tree
                // note: node is not a root // node is black
                Node_t* sibling = Father_t::__get_sibling(node);
                u8 children_count = Father_t::__count_children(sibling);

                if(children_count == 1){
                    if(sibling->rchild){
                        // situation 2.1.1
                        if(Father_t::__is_a_lchild(node)){
                            Father_t::__exchange_data(node,node->parent);
                            Father_t::__exchange_data(node->parent,sibling);
                            Father_t::__exchange_data(sibling,sibling->rchild);
                            node = sibling->rchild;
                            sibling->rchild = nullptr;
                        }else{
                            Father_t::__exchange_data(node,node->parent);
                            Father_t::__exchange_data(node->parent,sibling);
                            Father_t::__exchange_data(sibling,sibling->lchild);
                            node = sibling->lchild;
                            sibling->lchild = nullptr;
                        }
                        --this->size_;
                        return;
                    }else{
                        // situation 2.1.2
                        if(Father_t::__is_a_lchild(node)){
                            Father_t::__exchange_data(node->parent,sibling->lchild);
                            Father_t::__exchange_data(node,sibling->lchild);
                            node = sibling->lchild;
                            sibling->lchild = nullptr;
                        }else{
                            Father_t::__exchange_data(node->parent,sibling->rchild);
                            Father_t::__exchange_data(node,sibling->rchild);
                            node = sibling->rchild;
                            sibling->rchild = nullptr;
                        }
                        --this->size_;
                        return;
                    }
                }else if(children_count == 2){
                    // situation 2.1.3
                    if(Father_t::__is_a_lchild(node)){
                        Father_t::__exchange_data(node,node->parent);
                        Father_t::__exchange_data(node->parent,sibling);
                        Father_t::__exchange_data(sibling,sibling->rchild);
                        node->rchild = sibling->lchild;
                        sibling->lchild = nullptr;
                        node = sibling->rchild;
                    }else{
                        Father_t::__exchange_data(node,node->parent);
                        Father_t::__exchange_data(node->parent,sibling);
                        Father_t::__exchange_data(sibling,sibling->lchild);
                        node->lchild = sibling->rchild;
                        sibling->rchild = nullptr;
                        node = sibling->lchild;
                    }
                    --this->size_;
                    return;
                }else{
                    // sibling no children
                    if(Father_t::__is_a_lchild(node)) node->parent->lchild = nullptr;
                    else node->parent->rchild = nullptr;

                    __set_color(sibling,RedBlackTreeColor::RED);

                    Node_t* cur = node->parent;

                    loop:
                    if(__is_red(cur)){
                        __set_color(cur,RedBlackTreeColor::BLACK);
                        if(Father_t::__is_root(cur->parent)){
                            --this->size_;
                            return;
                        }else{
                            __set_color(cur->parent,RedBlackTreeColor::RED);
                            cur = cur->parent->parent;
                            goto loop;
                        }
                    }else{
                        --this->size_;
                        return;
                    }
                }
            }
        }
    }

};


} // namespace tcx

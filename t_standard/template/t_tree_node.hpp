#pragma once
#include <functional>

namespace tcx{

enum class RedBlackTreeColor: char{
    BLACK = 0,
    RED = 1,
};


template<typename T>
struct BinaryTreeNode{
    using Data_t = T;
    T* data;
    BinaryTreeNode* lchild;
    BinaryTreeNode* rchild;
    BinaryTreeNode* parent;
};


template<typename T>
struct RedBlackTreeNode{
    using Data_t = T;
    T* data;
    RedBlackTreeNode* lchild;
    RedBlackTreeNode* rchild;
    RedBlackTreeNode* parent;
    RedBlackTreeColor color;    // color
};

}
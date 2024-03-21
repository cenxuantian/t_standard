#pragma once
#include <functional>

namespace tcx{

enum class RedBlackTreeColor: char{
    BLACK = 0,
    RED = 1,
};


template<typename T>
struct __RedBlackTreeItem{};

template<typename T>
struct BinaryTreeNode{
    using Data_t = T;
    T* data;
    BinaryTreeNode* lchild;
    BinaryTreeNode* rchild;
    BinaryTreeNode* parent;
};


template<typename T>
struct BinaryTreeNode<__RedBlackTreeItem<T>>{
    using Data_t = T;
    T* data;
    BinaryTreeNode* lchild;
    BinaryTreeNode* rchild;
    BinaryTreeNode* parent;
    RedBlackTreeColor color;    // color
};

}
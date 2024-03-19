#pragma once
#include <functional>
#include <coroutine>

namespace tcx{

template<typename T>
struct BinaryTreeNode{
    T* data;
    BinaryTreeNode* lchild;
    BinaryTreeNode* rchild;
    BinaryTreeNode* parent;
};


}
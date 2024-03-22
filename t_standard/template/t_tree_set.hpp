#pragma once
#include "t_red_black_tree.hpp"

namespace tcx
{
template<typename T>
class TreeSet: public RedBlackTree<T,HowToDealWithDuplicate::AS_ERROR>{
public:
    using Self_t = TreeSet<T>;
    using Father_t = RedBlackTree<T,HowToDealWithDuplicate::AS_ERROR>;
    using GrandFather_t = SearchBinaryTree<RedBlackTreeNode<T>,HowToDealWithDuplicate::AS_ERROR>;

    TreeSet():Father_t(){}
    ~TreeSet(){}
    TreeSet(TreeSet&& _other)noexcept :Father_t(std::forward<TreeSet&&>(_other)){}

};
} // namespace tcx

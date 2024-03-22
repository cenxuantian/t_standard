#pragma once
#include "t_red_black_tree.hpp"
#include "../utility/t_pair.hpp"

namespace tcx
{
template<typename _T1,typename _T2>
class TreeMap: public RedBlackTree<Pair<_T1,_T2>,HowToDealWithDuplicate::AS_ERROR>{
public:
    using Self_t = TreeMap<_T1,_T2>;
    using Father_t = RedBlackTree<Pair<_T1,_T2>,HowToDealWithDuplicate::AS_ERROR>;
    using GrandFather_t = SearchBinaryTree<RedBlackTreeNode<Pair<_T1,_T2>>,HowToDealWithDuplicate::AS_ERROR>;
    using Node_t = RedBlackTreeNode<Pair<_T1,_T2>>;


    TreeMap():Father_t(){}
    ~TreeMap(){}
    TreeMap(TreeMap&& _other)noexcept :Father_t(std::forward<TreeMap&&>(_other)){}

    

};
} // namespace tcx

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
    using T = Node_t::Data_t;

    TreeMap():Father_t(){}
    ~TreeMap(){}
    TreeMap(TreeMap&& _other)noexcept :Father_t(std::forward<TreeMap&&>(_other)){}


    bool emplace(_T1&& key, _T2&& value)noexcept{
        return this->Father_t::emplace(Pair{std::move(key),std::move(value)});
    }

    // this function is not safe
    _T2& operator[](_T1 const& key){
        Node_t* cur = this->root_;
        
        loop:
            int comp_res = compare<__Faster_func_param_t<_T1>>(
                cur->data->first,key
            );
            if(comp_res == 1){
                // go to left
                cur = cur->lchild;
                goto loop;
            }else if(comp_res == -1){
                // go to right
                cur - cur->rchild;
                goto loop;
            }else return cur->data->second;

    }

private:



};
} // namespace tcx

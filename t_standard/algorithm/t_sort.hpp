#pragma once
#include "../template/t_iter_traits.hpp"
#include "t_math.hpp"

namespace tcx
{

template<typename T,typename = void>
constexpr bool __has_begin_func_v = false;

template<typename T>
constexpr bool __has_begin_func_v<T,std::void_t<decltype(std::declval<T&>().begin())>> = true;

template<typename T,typename = void>
constexpr bool __has_end_func_v = false;

template<typename T>
constexpr bool __has_end_func_v<T,std::void_t<decltype(std::declval<T&>().end())>> = true;

template<typename T>
constexpr bool __is_sortable_f(){
    if constexpr (std::is_array_v<T>) return true;

    if constexpr (__has_begin_func_v<T> && __has_end_func_v<T>){
        if constexpr(__is_iter_type_v<decltype(std::declval<T&>().end()),T>){
            return true;
        }
    }
    return false;
}

template<typename T>
constexpr bool __is_sortable_v = __is_sortable_f<T>();

template<typename ContentType>
using __to_CompareFuncInput_t = std::conditional_t<std::is_trivial_v<ContentType>,ContentType,ContentType const&>;

// O((1+n)n/2)
template<typename T>
void insert_sort(
    T& _in,
    bool(*func)(__to_CompareFuncInput_t<typename T::ContentType>,__to_CompareFuncInput_t<typename T::ContentType>) 
    = smaller_weak)
{
    if(_in.size()<=1) return;
    T out;
    auto in_b = _in.begin();
    auto in_e = _in.end();
    _in.give_to_front(in_b,out.begin());

    while(in_b!=in_e){
        bool changed = false;
        for(auto b=out.begin(),e=out.end();b!=e;++b){
            if (func(*in_b,*b)){
                _in.give_to_front(in_b,b);  //++in_b called, do not call it again
                changed = true;
                break;
            }
        }
        if(!changed){
            auto _last = out.last();
            _in.give_to_back(in_b,_last); //++in_b called, do not call it again
        }
    }
    
    _in = out.move();

    return;
}

template<typename T>
void fast_sort(
    T& _in,
    bool(*func)(__to_CompareFuncInput_t<typename T::ContentType>,__to_CompareFuncInput_t<typename T::ContentType>) 
    = smaller_weak)
{
    auto _in_size = _in.size();
    if(_in_size<=1) return;
    if(_in_size == 2){
        if(!(func(_in[0], _in[1]))) _in.reverse();
        return;
    }
    T left;  // std
    T& right = _in;
    auto r_b = right.begin();
    auto r_e = right.end();
    _in.give_to_front(r_b,left.begin());

    auto std_iter = left.begin();
    auto & std = *(std_iter);

    while(r_b!=r_e){
        if(func(*r_b,std )){
            right.give_to_front(r_b,std_iter);
        }else{
            ++r_b;
        }
    }
    
    fast_sort(left,func);
    fast_sort(right,func);
    left.merge(right.move());
    _in = left.move();

    return;
}

template<typename T>
void bubble_sort(
    T& _in,
    bool(*func)(__to_CompareFuncInput_t<typename T::ContentType>,__to_CompareFuncInput_t<typename T::ContentType>) 
    = smaller_weak)
{
    auto _in_size = _in.size();
    if(_in_size<=1) return;
    auto _std = _in.begin();
    auto _end = _in.end();
    bool changed = false;

    loop1:
    auto _comp = _std;   // use _std compare with "its next node" (_comp)
    ++_comp;

    loop2:
    if(_comp != _end && _std != _end){
        if(*_std>*_comp){
            T::swap(_std,_comp);
            ++_std;
            if(!changed) changed = true;
            goto loop1;
        }else{
            ++_comp;
            goto loop2;
        }
        
    }

    if(changed) bubble_sort(_in,func);
    
}

} // namespace tcx

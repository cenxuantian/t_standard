#pragma once
#include "../basic/t_meta.h"

namespace tcx
{

template<typename T>
T& max(T& _1,T& _2)noexcept{
    if constexpr(__has_opr_smaller<T>){
        if(_1<_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_smaller_eq<T>){
        if(_1<=_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger<T>){
        if(_2>_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_eq<T>){
        if(_2>=_1) return _2;
        else return _1;
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
T& min(T& _1,T& _2)noexcept{
    if constexpr(__has_opr_smaller<T>){
        if(_2<_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_smaller_eq<T>){
        if(_2<=_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger<T>){
        if(_1>_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_eq<T>){
        if(_1>=_2) return _2;
        else return _1;
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool bigger_strong(T _1,T _2) noexcept{
    if constexpr(__has_opr_bigger<T>){
        return _1>_2;
    }else if constexpr(__has_opr_smaller_eq<T>){
        return !(_1<=_2);
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool smaller_strong(T _1,T _2) noexcept{
    if constexpr(__has_opr_smaller<T>){
        return _1<_2;
    }else if constexpr(__has_opr_bigger_eq<T>){
        return !(_1>=_2);
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool bigger_weak(T _1,T _2) noexcept{
    if constexpr(__has_opr_bigger<T>){
        return _1>_2;
    }else if constexpr(__has_opr_smaller_eq<T>){
        return !(_1<=_2);
    }
    else if constexpr(__has_opr_smaller<T>){
        return !(_1<_2);
    }
    else if constexpr(__has_opr_bigger_eq<T>){
        return _1>=_2;
    }
    else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool smaller_weak(T _1,T _2) noexcept{
    if constexpr(__has_opr_smaller<T>){
        return _1<_2;
    }else if constexpr(__has_opr_bigger_eq<T>){
        return !(_1>=_2);
    }
    else if constexpr(__has_opr_bigger<T>){
        return !(_1>_2);
    }
    else if constexpr(__has_opr_smaller_eq<T>){
        return _1<=_2;
    }
    else{
        static_assert(false,"type T imcomparable");
    }
}

} // namespace tcx

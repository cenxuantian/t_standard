#pragma once
#include "../basic/t_meta.h"

namespace tcx
{

template<typename T>
T& max(T& _1,T& _2)noexcept{
    if constexpr(__has_opr_smaller_v<T>){
        if(_1<_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_smaller_eq_v<T>){
        if(_1<=_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_v<T>){
        if(_2>_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_eq_v<T>){
        if(_2>=_1) return _2;
        else return _1;
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
T& min(T& _1,T& _2)noexcept{
    if constexpr(__has_opr_smaller_v<T>){
        if(_2<_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_smaller_eq_v<T>){
        if(_2<=_1) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_v<T>){
        if(_1>_2) return _2;
        else return _1;
    }else if constexpr(__has_opr_bigger_eq_v<T>){
        if(_1>=_2) return _2;
        else return _1;
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool bigger_strong(T _1,T _2) noexcept{
    if constexpr(__has_opr_bigger_v<T>){
        return _1>_2;
    }else if constexpr(__has_opr_smaller_eq_v<T>){
        return !(_1<=_2);
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool smaller_strong(T _1,T _2) noexcept{
    if constexpr(__has_opr_smaller_v<T>){
        return _1<_2;
    }else if constexpr(__has_opr_bigger_eq_v<T>){
        return !(_1>=_2);
    }else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool bigger_weak(T _1,T _2) noexcept{
    if constexpr(__has_opr_bigger_v<T>){
        return _1>_2;
    }else if constexpr(__has_opr_smaller_eq_v<T>){
        return !(_1<=_2);
    }
    else if constexpr(__has_opr_smaller_v<T>){
        return !(_1<_2);
    }
    else if constexpr(__has_opr_bigger_eq_v<T>){
        return _1>=_2;
    }
    else{
        static_assert(false,"type T imcomparable");
    }
}

template<typename T>
bool smaller_weak(T _1,T _2) noexcept{
    if constexpr(__has_opr_smaller_v<T>){
        return _1<_2;
    }else if constexpr(__has_opr_bigger_eq_v<T>){
        return !(_1>=_2);
    }
    else if constexpr(__has_opr_bigger_v<T>){
        return !(_1>_2);
    }
    else if constexpr(__has_opr_smaller_eq_v<T>){
        return _1<=_2;
    }
    else{
        static_assert(false,"type T imcomparable");
    }
}

// returns -1 when _1 smaller than _2
// returns 0 when _1 equal than _2
// returns 1 when _1 bigger than _2
template<typename T>
int compare(T _1,T _2) noexcept{
    if constexpr(__has_opr_bigger_v<T>){
        if(_1>_2) return 1;
        else{// < or =
            if constexpr(__has_opr_bigger_eq_v<T>){
                if(_1>=_2) return 0;
                else return -1;
            }else if constexpr(__has_opr_eq_v<T>){
                if(_1==_2) return 0;
                else return -1;
            }else if constexpr(__has_opr_smaller_v<T>){
                if(_1<_2)return -1;
                else return 0;
            }else{
                static_assert(false,"cannot resolve comparison");
            }
        }
    }
    
    else if constexpr (__has_opr_smaller_v<T>){
        if(_1<_2) return -1;
        else{
            if constexpr(__has_opr_smaller_eq_v<T>){
                if(_1<=_2) return 0;
                else return 1;
            }else if constexpr(__has_opr_eq_v<T>){
                if(_1==_2) return 0;
                else return 1;
            }else{
                static_assert(false,"cannot resolve comparison");
            }
        }
    }

    else if constexpr (__has_opr_eq_v<T>){
        if(_1==_2) return 0;
        else{
            if constexpr(__has_opr_smaller_eq_v<T>){
                if(_1<=_2) return -1;
                else return 1;
            }else if constexpr(__has_opr_bigger_eq_v<T>){
                if(_1>=_2)return 1;
                else return 0;
            }else{
                static_assert(false,"cannot resolve comparison");
            }
        }
    }
    
    else if constexpr(__has_opr_smaller_eq_v<T> && __has_opr_bigger_eq_v<T>){
        if(!(_1<=_2)) return 1;
        else if(!(_1>=_2))return -1;
        else return 0;
    }
    else {
        static_assert(false,"cannot resolve comparison");
    }
    
    return 0;
}


template<typename T,typename = void>
constexpr bool __is_comparable_v = false;

template<typename T>
constexpr bool __is_comparable_v<T,std::void_t<decltype(compare(std::declval<T&>(),std::declval<T&>()))>> = true;

} // namespace tcx

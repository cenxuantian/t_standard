#pragma once
#include "../basic/t_class_helper.h"
#include "./basic/t_meta.h"
namespace tcx
{
template<typename T1,typename T2>
requires(__completely_moveable_v<T1,T2>)
class Pair : public NonCopyble{
public:
    T1 first;
    T2 second;

    Pair(T1&& _d1, T2&& _d2)
    :first(std::forward<T1&&>(_d1))
    ,second(std::forward<T1&&>(_d2)){}

    Pair(Pair&& _other) noexcept {
        first = std::move(_other.first);
        second = std::move(_other.second);
    }

    Pair copy() 
    const noexcept 
    requires(std::is_copy_constructible_v<T1> && std::is_copy_constructible_v<T2>)
    {   
        // copy
        T1 _1{this->first};
        T2 _2{this->second};
        return {std::move(_1),std::move(_2)};
    }

    Pair&& move() noexcept{
        return std::move(*this);
    }

    ~Pair(){}
    



};
} // namespace tcx

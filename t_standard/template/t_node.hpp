#pragma once
#include "../basic/t_meta.h"

namespace tcx
{
template<typename T>
struct SingleNode{
    using ContentType = T;

    T* data_ = nullptr;
    SingleNode<T>* child_ = nullptr;
};

template<typename T>
struct DoubleNode{
    using ContentType = T;
    T* data_ = nullptr;
    DoubleNode<T>* child_ = nullptr;
    DoubleNode<T>* father_ = nullptr;
};


template<typename T>
constexpr bool __check_is_node_type(){
    if constexpr (__has_content_type<T>){
        return __belongs_to<T(
            SingleNode<typename T::ContentType>,
            DoubleNode<typename T::ContentType>
        )>;
    }else{
        return false;
    }
}

template<typename T>
constexpr bool __check_is_SingleNode_type(){
    if constexpr (__has_content_type<T>){
        return __belongs_to<T(
            SingleNode<typename T::ContentType>
        )>;
    }else{
        return false;
    }
}

template<typename T>
constexpr bool __check_is_DoubleNode_type(){
    if constexpr (__has_content_type<T>){
        return __belongs_to<T(
            DoubleNode<typename T::ContentType>
        )>;
    }else{
        return false;
    }
}


template<typename T>
constexpr bool __is_node_t = __check_is_node_type<T>();

template<typename T>
constexpr bool __is_SingleNode_t = __check_is_SingleNode_type<T>();

template<typename T>
constexpr bool __is_DoubleNode_t = __check_is_DoubleNode_type<T>();

} // namespace tcx

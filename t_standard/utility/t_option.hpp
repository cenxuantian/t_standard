#pragma once

#include "../basic/t_class_helper.h"
#include "../basic/t_meta.h"

#include <functional>
#include <algorithm>
#include <cstring>


namespace tcx
{

template<typename _Type>
class Option: public NonCopyble{
protected:
    unsigned char has_val_;
    _Type val_;

    explicit Option(unsigned char _has_val,_Type&& _val)
    noexcept(std::is_nothrow_move_constructible_v<_Type>)
    requires(std::is_move_constructible_v<_Type>)
    :has_val_(has_val_)
    ,val_(std::move(_val)){

    }


    explicit Option(unsigned char _has_val,_Type const& _val)
    noexcept(std::is_nothrow_copy_constructible_v<_Type>)
    requires(std::is_copy_constructible_v<_Type>)
    :has_val_(has_val_)
    ,val_(_val){

    }

    // default constructor for _Type
    explicit Option(unsigned char _has_val)
    noexcept(std::is_nothrow_default_constructible_v<_Type>)
    requires(std::is_default_constructible_v<_Type>)
    :has_val_(has_val_)
    ,val_(){

    }

    // default constructor
    explicit Option()
    noexcept(std::is_nothrow_default_constructible_v<_Type>)
    requires(std::is_default_constructible_v<_Type>)
    :has_val_(0)
    ,val_(){

    }

public:

    // move constrcutor
    explicit Option(Option&& _other)
    noexcept(std::is_nothrow_move_constructible_v<_Type>)
    requires(std::is_move_constructible_v<_Type>)
    :has_val_(_other.has_val_)
    ,val_(std::move(_other.val_))// call move constructor
    {
        _other.has_val_ = 0;
    }

    Option& operator=(Option&& _other) = delete;


    // copy from other Option Create a new Option
    static Option copy_from(Option const& _other)
    noexcept(noexcept(Option(_other.has_val_,_other.val_)))
    requires(std::is_copy_constructible_v<_Type>) 
    {
        return Option(_other.has_val_,_other.val_);
    }

    // move from other Option using left value reference
    static Option move_from(Option& _other)
    noexcept(noexcept(Option(_other.has_val_,std::move(_other.val_))))
    requires(std::is_move_constructible_v<_Type>)
    {
        return Option(_other.has_val_,std::move(_other.val_));
    }
    
    // move from other Option using rvalue reference
    static Option move_from(Option&& _other)
    noexcept(noexcept(Option(_other.has_val_,std::forward<Option>(_other.val_))))
    requires(std::is_move_constructible_v<_Type>)
    {
        return Option(_other.has_val_,std::forward<Option>(_other.val_));
    }

    // create a new Option using rvalue reference
    static Option Some(_Type&& _val)
    noexcept(noexcept(Option(1,std::forward<_Type>(_val))))
    requires(std::is_move_constructible_v<_Type>)
    {
        return Option(1,std::forward<_Type>(_val));
    }

    // create a new Option using lvalue reference
    // this function can only be used for types that is copyable but nonmoveable
    // if need to craete an Option for moveable types by using its copy constructor
    // plz use the copy_from function instead
    static Option Some(_Type const& _val)
    noexcept(noexcept(Option(1,_val)))
    requires(!std::is_move_constructible_v<_Type> && std::is_copy_constructible_v<_Type>)
    {
        return Option(1,_val);
    }

    // use defualt constructor to create a "Some" Option
    static Option Some_default()
    noexcept(noexcept(Option(1)))
    {
        return Option(1);
    }

    // use copy constructor to create a "Some" Option
    static Option Some_copy_from(_Type const& _val)
    noexcept(noexcept(Option(1,_val)))
    {
        return Option(1,_val);
    }

    // use default constructor to create a "None" Option
    static Option None()
    noexcept(noexcept(Option(0)))
    requires(std::is_default_constructible_v<_Type>)
    {
        return Option(0);
    }

    // this function is enable for those types that cannot be construct by default constructor
    static Option None()
    noexcept(noexcept(Option(0,std::move(std::declval<_Type&>()))))
    requires(!std::is_default_constructible_v<_Type>)
    {
        unsigned char _fake[sizeof(_Type)]{0};
        _Type& _temp = (*(_Type*)(_fake));
        return Option(0,std::move(_temp));
    }


    bool is_some()const noexcept {
        return has_val_==1;
    }
    bool is_none()const noexcept {
        return has_val_==0;
    }
    _Type& unwrap() noexcept{
        return val_;
    }
    _Type const& unwrap()const noexcept{
        return val_;
    }
    _Type& unwrap_or(_Type const& _val) noexcept{
        if(has_val_) return val_;
        else return const_cast<_Type&>(_val);
    }
    _Type const& unwrap_or(_Type const& _val)const noexcept{
        if(has_val_) return val_;
        else return _val;
    }

};

} // namespace tcx

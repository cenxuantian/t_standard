#pragma once

#include "../basic/t_class_helper.h"
#include "../basic/t_meta.h"

namespace tcx
{
template<typename _Type_1,typename _Type_2>
class Switch: public NonCopyble{
protected:
    unsigned char is_first_ = 1;
    void* data_ = nullptr;

    explicit Switch()
    noexcept(std::is_nothrow_default_constructible_v<_Type_1> && noexcept(new _Type_1()))
    requires(std::is_default_constructible_v<_Type_1>)
    :data_((void*)new _Type_1())
    ,is_first_(1){}

    explicit Switch(_Type_1 const& _val)
    noexcept(std::is_nothrow_copy_constructible_v<_Type_1>)
    requires(std::is_copy_constructible_v<_Type_1>)
    :data_((void*)new _Type_1(_val))
    ,is_first_(1){}

    explicit Switch(_Type_1 && _val)
    noexcept(std::is_nothrow_move_constructible_v<_Type_1>)
    requires(std::is_move_constructible_v<_Type_1>)
    :data_((void*)new _Type_1(std::forward<_Type_1>(_val)))
    ,is_first_(1){}

    explicit Switch(_Type_2 const& _val)
    noexcept(std::is_nothrow_copy_constructible_v<_Type_2>)
    requires(std::is_copy_constructible_v<_Type_2>)
    :data_((void*)new _Type_2(_val))
    ,is_first_(1){}

    explicit Switch(_Type_2 && _val)
    noexcept(std::is_nothrow_move_constructible_v<_Type_2>)
    requires(std::is_move_constructible_v<_Type_2>)
    :data_((void*)new _Type_2(std::forward<_Type_2>(_val)))
    ,is_first_(1){}

public:
    explicit Switch(Switch && _other) noexcept
    :data_(_other.data_)
    ,is_first_(_other.is_first_){
        _other.is_first_ = 1;
        _other.data_ = nullptr;
    }

    Switch& operator=(Switch&& _other) = delete;

    // default constructor for _Type_1
    static Switch First()
    noexcept(Switch())
    requires(std::is_default_constructible_v<_Type_1>)
    {
        return Switch();
    }

    static Switch First(_Type_1&& _val)
    noexcept(noexcept(Switch(std::forward<_Type_1>(_val))))
    requires(std::is_move_constructible_v<_Type_1>)
    {
        return Switch(std::forward<_Type_1>(_val));
    }

    static Switch First(_Type_1 const& _val)
    noexcept(noexcept(Switch(_val)))
    requires(std::is_copy_constructible_v<_Type_1> && !std::is_move_constructible_v<_Type_1>)
    {
        return Switch(_val);
    }

    static Switch Second()
    noexcept(Switch())
    requires(std::is_default_constructible_v<_Type_2>)
    {
        return Switch(_Type_2{});
    }

    static Switch Second(_Type_2&& _val)
    noexcept(noexcept(Switch(std::forward<_Type_2>(_val))))
    requires(std::is_move_constructible_v<_Type_2>)
    {
        return Switch(std::forward<_Type_2>(_val));
    }

    static Switch Second(_Type_2 const& _val)
    noexcept(noexcept(Switch(_val)))
    requires(std::is_copy_constructible_v<_Type_2> && !std::is_move_constructible_v<_Type_2>)
    {
        return Switch(_val);
    }

    static Switch copy_from(Switch const& _other)
    noexcept(noexcept(Switch(_other)))
    {
        return Switch(_other);
    }
    static Switch move_from(Switch && _other)
    noexcept(noexcept(Switch(std::forward<Switch>(_other))))
    {
        return Switch(std::forward<Switch>(_other));
    }
    static Switch move_from(Switch const& _other)
    noexcept(noexcept(Switch(std::move(_other))))
    {
        return Switch(std::move(_other));
    }

    bool is_first()noexcept{
        return is_first_==1;
    }
    bool is_second()noexcept{
        return is_first_ !=0;
    }
    _Type_1& get_first()noexcept{
        return *(static_cast<_Type_1*>(data_));
    }
    _Type_2& get_second()noexcept{
        return *(static_cast<_Type_2*>(data_));
    }
};

} // namespace tcx

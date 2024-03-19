#pragma once
#include <coroutine>


namespace tcx{

template<typename T>
class OnlyInitSusCoro{
public:
    struct promise_type{
        OnlyInitSusCoro get_return_object(){
            return OnlyInitSusCoro(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        auto initial_suspend()noexcept {return std::suspend_never{};};
        auto final_suspend()noexcept {return std::suspend_always{};};
        auto yield_value()noexcept{return std::suspend_never{};}
        void return_value() noexcept{}
        void unhandled_exception(){}
    };

    std::coroutine_handle<promise_type> handle;
    explicit OnlyInitSusCoro(std::coroutine_handle<promise_type> const& h):handle(h){}
    ~OnlyInitSusCoro(){handle.destroy();}
};

}
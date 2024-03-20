#pragma once
#include <coroutine>


namespace tcx{

template<typename T>
class OnlyFinalSusCoro{
public:
    struct promise_type{
        OnlyFinalSusCoro get_return_object(){
            return OnlyFinalSusCoro(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        auto initial_suspend()noexcept {return std::suspend_never{};};
        auto final_suspend()noexcept {return std::suspend_always{};};
        auto yield_value()noexcept{return std::suspend_never{};}
        void return_value() noexcept{}
        void unhandled_exception(){}
    };

    std::coroutine_handle<promise_type> handle;
    explicit OnlyFinalSusCoro(std::coroutine_handle<promise_type> const& h):handle(h){}
    ~OnlyFinalSusCoro(){handle.destroy();}
};

}
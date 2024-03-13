#include "../basic/t_class_helper.h"

#include <cstdint>
#include <type_traits>
#include <utility>
#include <atomic>

namespace tcx
{
template<typename _Type,size_t _Size>
class AtomicRingQueue:public NonCopyble{
public:

    AtomicRingQueue()=default;
    ~AtomicRingQueue()=default;

    AtomicRingQueue(AtomicRingQueue&&)=default;
    AtomicRingQueue& operator=(AtomicRingQueue&&)=default;

    AtomicRingQueue copy(){
        AtomicRingQueue res;
        memcpy(res.data);
        res.impl.store(this->impl);
        return res.move();
    }

    AtomicRingQueue&& move() noexcept {return std::move(*this);}



    void push(_Type const& _in_ldata){
        _update_data_copy(_in_ldata,_atomic_push_pos());
    }
    
    // if return ~0ULL if not safe
    size_t safe_pop_pos() noexcept {
        return _atomic_pop_pos();
    }
    
    _Type safe_pop(size_t pos) noexcept {
        return std::move(data[pos]);
    }
    
    // this function can only get the current size
    // if other thread called push or safe_pop_pos
    // the size could be different
    size_t size()const noexcept{
        return impl.load().cur_size;
    }
    
    // get the capasity
    size_t capacity()const noexcept{
        return _Size;
    }

    // this function will not really clear the data,
    // it will just change the marks of head and tail position
    // so this function is fast
    void clear() noexcept{
        impl.store(_AtomicRingQueueImpl());
    }

private:
    struct _AtomicRingQueueImpl{
        size_t cur_start = 0;
        size_t cur_end = 0;
        size_t cur_size = 0;
    };

    _Type data[_Size];  // ring buffer
    volatile std::atomic<_AtomicRingQueueImpl> impl;
    
    void _update_data_copy(_Type const& _in_data,size_t index){
        if constexpr(std::is_move_assignable_v<_Type>) 
            data[index] = _in_data;
        else memcpy(data+index,&_in_data,sizeof(_Type));
    }
    
    // return 0 the impl after push
    // return 1 the pos should be modified
    static std::pair<_AtomicRingQueueImpl,size_t> _next_push(_AtomicRingQueueImpl const& _before_impl) noexcept{
        std::pair<_AtomicRingQueueImpl,size_t> res{_before_impl,0};

        if(res.first.cur_end==res.first.cur_start && res.first.cur_size!=0){
            res.first.cur_size--;
            res.first.cur_start++;
            res.first.cur_start = res.first.cur_start==_Size?0:res.first.cur_start;
        }
        res.second = res.first.cur_end;
        res.first.cur_end++;
        res.first.cur_end = res.first.cur_end==_Size?0:res.first.cur_end;
        res.first.cur_size++;
        return res;
    }
    // return 0 the impl after pop
    // return 1 the pos should be returned
    static std::pair<_AtomicRingQueueImpl,size_t> _next_pop(_AtomicRingQueueImpl const& _before_impl) noexcept{
        if(!_before_impl.cur_size){
            return {_AtomicRingQueueImpl(),~(0ULL)};
        }
        std::pair<_AtomicRingQueueImpl,size_t> res{_before_impl,0};
        res.second = res.first.cur_start;
        res.first.cur_start++;
        res.first.cur_start = res.first.cur_start==_Size?0:res.first.cur_start;
        res.first.cur_size--;
        return res;
    }

    // get the pos that needed to be pushed
    // and update the impl
    size_t _atomic_push_pos() noexcept {
        _AtomicRingQueueImpl save_impl;
        std::pair<_AtomicRingQueueImpl,size_t> _pair_res;
        while(1){
            save_impl= impl.load();
            _pair_res= _next_push(save_impl);
            if(impl.compare_exchange_strong(save_impl,_pair_res.first)){
                break;
            }
        }
        return _pair_res.second;
    }

    // get the pos that needed to be returned
    // and update the impl
    size_t _atomic_pop_pos() noexcept{
        _AtomicRingQueueImpl save_impl;
        std::pair<_AtomicRingQueueImpl,size_t> _pair_res;
        while(1){
            save_impl= impl.load();
            _pair_res= _next_pop(save_impl);
            if(impl.compare_exchange_strong(save_impl,_pair_res.first)){
                break;
            }
        }
        return _pair_res.second;
    }


};

}


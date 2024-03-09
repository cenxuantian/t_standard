#pragma once
#include "t_arr_buffer.hpp"
#include <cstring>
namespace tcx
{

// used for byte 
// this class has push pop functions
template<typename T>
concept IsByte = std::is_same_v<T,u8> || std::is_same_v<T,i8> || sizeof(T) == 1;

template<typename T>
concept IsBytePointer = 
    (std::is_pointer_v<T> && IsByte<std::decay_t<std::remove_pointer_t<T>>>)
    ||
    (std::is_array_v<T> && IsByte<std::decay_t<decltype(std::declval<T&>()[0])>>);

class ByteBuffer :public ArrBuffer<byte> {

public:

// constructors
    ByteBuffer()noexcept :ArrBuffer<byte>(){}

    ~ByteBuffer(){}

    ByteBuffer(ByteBuffer&& _other) noexcept :ArrBuffer<byte>(std::forward<ByteBuffer&&>(_other)) {}

    template<typename T>
    requires(std::is_base_of_v<T,ByteBuffer>)
    ByteBuffer(T&& _other) noexcept :ArrBuffer<byte>(std::forward<T&&>(_other)) {}

    template<typename T>
    requires IsByte<T>
    ByteBuffer(std::initializer_list<T> const& _ini_list) noexcept :ArrBuffer<byte>(_ini_list){}

    template<typename T>
    requires IsByte<T>
    ByteBuffer(const T* _c_arr) noexcept : ArrBuffer<byte>((const byte*)(_c_arr),(usize)strlen((const char*)(_c_arr))){}

    template<typename T>
    requires IsByte<T>
    ByteBuffer(const T* _c_arr,usize _size) noexcept : ArrBuffer<byte>((const byte*)(_c_arr),_size){}


// operation functions
    template<typename T>
    requires IsByte<T>
    ByteBuffer& push_back(T const* _c_arr)noexcept{
        usize ava_size =this->space_ - this->data_size_;
        usize size_cost = strlen(_c_arr);
        if(ava_size<size_cost){
            this->__space_grow(size_cost);
        }
        memcpy(this->data_ + this->data_size_ ,_c_arr, size_cost);
        this->data_size_+=size_cost;
        return *this;
    }

    template<typename T>
    requires IsByte<T>
    ByteBuffer& push_back(T const* _c_arr, usize _size)noexcept{
        usize ava_size =this->space_ - this->data_size_;
        if(ava_size<_size){
            this->__space_grow(_size);
        }
        memcpy(this->data_ + this->data_size_ ,_c_arr, _size);
        this->data_size_+=_size;
        return *this;
    }

    ByteBuffer& push_back(byte _c)noexcept{
        this->ArrBuffer<byte>::push_back(_c);
        return *this;
    }

    ByteBuffer& push_back(ByteBuffer const& _another)noexcept{
        this->ArrBuffer<byte>::push_back(_another);
        return *this;
    }

    template<typename T>
    requires IsByte<T> || IsBytePointer<T> || ( std::is_base_of_v<ArrBuffer<byte>,T>)
    ByteBuffer operator+(T const& _in_data) noexcept{
        ByteBuffer res = this->copy();
        res.push_back(_in_data);
        return res.move();
    }

    template<typename T>
    requires IsByte<T> || IsBytePointer<T> || ( std::is_base_of_v<ArrBuffer<byte>,T>)
    ByteBuffer& operator+=(T const& _in_data) noexcept{
        return this->push_back(_in_data);
    }

    ByteBuffer& push_front(byte _c)noexcept{
        this->ArrBuffer<byte>::push_front(_c);
        return *this;
    }

    ByteBuffer& push_front(ByteBuffer const& _another)noexcept{
        this->ArrBuffer<byte>::push_front(_another);
        return *this;
    }

    template<typename T>
    requires IsByte<T>
    ByteBuffer& push_front(T const* _c_arr)noexcept{

        usize ava_size =this->space_ - this->data_size_;
        usize size_cost = strlen(_c_arr);
        if(ava_size<size_cost){
            this->__space_grow(size_cost);
        }
        memcpy(this->data_ + size_cost,this->data_,this->data_size_);
        memcpy(this->data_,_c_arr, size_cost);
        this->data_size_+=size_cost;

        return *this;
    }

    template<typename T>
    requires IsByte<T>
    bool pop_back(T* _dist, usize _size) {
        if(this->read_from_tail((byte*)_dist,_size)){
            this->erase_from_tail(_size);
            return true;
        }else{
            return false;
        }
    }

    template<typename T>
    requires IsByte<T>
    bool pop_front(T* _dist, usize _size) {
        if(this->read_from_head((byte*)_dist,_size)){
            this->erase_from_head(_size);
            return true;
        }else{
            return false;
        }
    }

    ByteBuffer sub(usize _start,usize _size) const {
        return {this->data_+_start,_size};
    }

    template<typename T>
    requires IsByte<T>
    usize find_first_of(T _c)const noexcept{
        for(usize i=0;i< this->data_size_;i++){
            if(this->data_[i] == static_cast<byte>(_c)){
                return i;
            }
        }
        return npos;
    }

    
    template<typename T>
    requires IsByte<T>
    usize find_last_of(T _c)const noexcept{
        for(usize i=this->data_size_-1;i<= 0;i--){
            if(this->data_[i] == static_cast<byte>(_c)){
                return i;
            }
        }
        return npos;
    }


};


} // namespace tcx

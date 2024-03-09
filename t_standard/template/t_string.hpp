#pragma once
#include "t_byte_buffer.hpp"

namespace tcx
{
class String:public ByteBuffer{

public:
// constructors
    String():ByteBuffer(){}
    ~String(){}
    String(String&& _other) noexcept :ByteBuffer(std::forward<String&&>(_other)) {}

    template<typename T>
    requires(std::is_base_of_v<T,String>)
    String(T&& _other) noexcept :ByteBuffer(std::forward<T&&>(_other)) {}

    template<typename T>
    requires IsByte<T>
    String(const T* _str)noexcept:ByteBuffer(_str){}

// operator functions
    String substr(usize _start,usize _size) const {
        return this->sub(_start,_size);
    }

    const char* c_str() const noexcept{
        if(this->data_size_>=this->space_){
            const_cast<String*>(this)->__space_grow(1);
        }
        this->data_[this->data_size_]=0;
        return (const char*)this->data_;
    }

    ByteStream& stream() noexcept{
        return *(reinterpret_cast<ByteStream*>(this));
    }
    
    ByteStream const& stream() const noexcept{
        return *(reinterpret_cast<ByteStream const*>(this));
    }
};
} // namespace tcx

#pragma once
#include "t_byte_buffer.hpp"
#include "../algorithm/t_math.hpp"

namespace tcx{


class ByteStream:public ByteBuffer{
public:
// constructors
    ByteStream():ByteBuffer(){}
    ~ByteStream(){}
    ByteStream(ByteStream&& _other) noexcept :ByteBuffer(std::forward<ByteStream&&>(_other)) {}

    template<typename T>
    requires(std::is_base_of_v<T,ByteStream>)
    ByteStream(T&& _other) noexcept :ByteBuffer(std::forward<T&&>(_other)) {}

    template<typename T>
    requires IsByte<T>
    ByteStream(const T* _str)noexcept:ByteBuffer(_str){}

// operator functions
    template<typename T>
    ByteStream& operator<<(T&& _any) noexcept{
        this->ByteBuffer::push_back(std::forward<T&&>(_any));
        return *this;
    };

    ByteStream& read_some(void* _dist,usize _size) {
        this->pop_front((const byte*)_dist,_size);
        return *this;
    }

    usize read_until(void* _dist, byte _c){
        usize tar_pos = find_first_of(_c);
        if(tar_pos == npos) return 0;
        else{
            usize res = tar_pos+1;
            read_some(_dist,res);
            return res;
        }

    }

    template<typename T>
    requires(std::is_base_of_v<ByteBuffer,T>)
    ByteStream& operator>>(T& _tar){
        usize tar_pos_1 = find_first_of(' ');
        usize tar_pos_2 = find_first_of('\n');
        usize& min_pos = min(tar_pos_1,tar_pos_2);
        _tar.clear();

        if(min_pos == npos){
            _tar.push_back(this->data_,this->data_size_);
            this->clear();
        }else{
            _tar.push_back(this->data_,min_pos);
            this->erase_from_head(min_pos+1);
        }
        return *this;
    }

    bool eof()const noexcept{
        return this->data_size_<=0;
    }

    String& str() noexcept{
        return *(reinterpret_cast<String*>(this));
    }

    String const& str() const noexcept{
        return *(reinterpret_cast<const String*>(this));
    }


};


}
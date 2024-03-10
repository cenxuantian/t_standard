#pragma once
#include <cstring>
#include "../basic/t_int.h"
#include "../basic/t_class_helper.h"
#include "t_iter_traits.hpp"

namespace tcx
{
// pre def
template<typename T> class DynamicArr;
template<typename T> class ArrBuffer;
class ByteBuffer;
class String;
class ByteStream;

template<typename T>
class DynamicArrIter:public TemplateIterTraits<DynamicArr<T>>{
public:
    friend class DynamicArr<T>;
    friend class ArrBuffer<T>;

    using TraitsType = TemplateIterTraits<DynamicArr<T>>;
    using OwnerType = DynamicArr<T>;
    using ContentType = DynamicArr<T>::ContentType;


    DynamicArrIter(OwnerType& _owner,usize _pos):TraitsType(_owner),pos_(_pos){}
    virtual DynamicArrIter& operator++()override{
        ++pos_;
        return *this;
    };
    virtual T& operator*() override {
        return TraitsType::owner_[pos_];
    };
    virtual bool operator!=(TraitsType const& _other) override{
        DynamicArrIter const& _other_1 = dynamic_cast<DynamicArrIter const&>(_other);
        return _other_1.pos_ != pos_;
    }
private:
    usize pos_;
    
};

template<typename T>
class DynamicArrIterConst:public TemplateIterConstTraits<DynamicArr<T>>{
public:
    using TraitsType = TemplateIterConstTraits<DynamicArr<T>>;
    using OwnerType = DynamicArr<T>;
    using ContentType = DynamicArr<T>::ContentType;

    DynamicArrIterConst(OwnerType const& _owner,usize _pos):TraitsType(_owner),pos_(_pos){}
    virtual DynamicArrIterConst& operator++()override{
        ++pos_;
        return *this;
    };
    virtual ContentType const& operator*() override {
        return TraitsType::owner_[pos_];
    };
    virtual bool operator!=(TraitsType const& _other) override{
        DynamicArrIterConst const& _other_1 = dynamic_cast<DynamicArrIterConst const&>(_other);
        return _other_1.pos_ != pos_;
    }
private:
    usize pos_;
};

template<typename T>
class DynamicArr:public NonCopyble{
public:
    using ContentType = T;
    // default constructor
    DynamicArr() noexcept 
    :data_(nullptr),space_(0){}
    
    ~DynamicArr() noexcept {
        __shrink_to_zero();
    }
    // move constructor
    DynamicArr(DynamicArr && _other) noexcept
    :data_(_other.data_)
    ,space_(_other.space_)
    {
        _other.space_ = 0;
        _other.data_ = nullptr;
    }

    DynamicArr& operator=(DynamicArr && _other) noexcept
    {
        __shrink_to_zero();
        data_ = _other.data_;
        space_ = _other.space_;
        _other.space_ = 0;
        _other.data_ = nullptr;
        return *this;
    }
    
    DynamicArr<T> copy() const noexcept{
        DynamicArr<T> res;
        if(space_){
            res.__space_grow(space_);
            memcpy(res.data_,data_,space_);
        }
        return res.move();
    }
    
    virtual DynamicArr<T>&& move() noexcept{
        return std::move(*this);
    }
    // construct by c array
    DynamicArr(const T* _data,usize _space)noexcept 
    :data_(nullptr),space_(_space)
    {
        data_ = new T[_space];
        memcpy(data_,_data,_space);
    }
    // construct by initializer_list
    DynamicArr(std::initializer_list<T> const& _ini_list) noexcept
    :data_(new T[_ini_list.size()]),space_(_ini_list.size())
    {
        if constexpr(std::is_move_assignable_v<T>){
            usize pos = 0;
            for(const auto&i:_ini_list){
                data_[pos] = std::move(i);
                pos++;
            }
        }else if constexpr(std::is_copy_assignable_v<T>){
            usize pos = 0;
            for(const auto&i:_ini_list){
                data_[pos] = i;
                pos++;
            }
        }else{
            static_assert(false,"T is not copyable or moveable");
        }
    }



    T& operator[](usize _pos){
        return data_[_pos];
    }
    
    T const& operator[](usize _pos) const{
        return data_[_pos];
    }

    const usize size() const noexcept{
        return space_;
    }

    T const* data()const noexcept{
        return data_;
    }

    T* data() noexcept{
        return data_;
    }



    DynamicArrIter<T> begin(){
        return {*this,0};
    }

    DynamicArrIter<T> end(){
        return {*this,space_};
    }

    DynamicArrIterConst<T> begin() const{
        return {*this,0};
    }

    DynamicArrIterConst<T> end() const{
        return {*this,space_};
    }

    bool operator==(DynamicArr<T> const& _other) const noexcept{
        if(_other.size()!=this->size())return false;
        usize pos = 0;
        for(const auto&i:_other){
            if(i != this->operator[](pos)) return false;
            ++pos;
        }
        return true;
    }

protected:
    // grow
    void __space_grow(usize _size) noexcept {
        if(space_){
            T* _new_data = new T[space_+_size];
            memcpy(_new_data,data_,space_);
            delete[] data_;
            data_ = _new_data;
            space_ = space_+_size;
        }else{
            data_ = new T[_size];
            space_ = _size;
        }
    }
    // shrink
    void __space_shrink(usize _size) noexcept{
        if(space_ >_size){
            T* _new_data = new T[space_-_size];
            memcpy(_new_data,data_,_size);
            delete[] data_;
            data_ = _new_data;
            space_ = space_-_size;
        }else{
            delete[] data_;
            data_ = nullptr;
            space_ = 0;
        }
    }

    void __shrink_to_zero() noexcept{
        if(data_){
            delete[] data_;
            data_ = nullptr;
            space_ = 0;
        }
    }

    T* data_;
    usize space_;
};

} // namespace tcx

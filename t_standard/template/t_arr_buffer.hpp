#pragma once
#include "t_dynamic_arr.hpp"

namespace tcx
{


// this class can grow up and reserve size by itself
template<typename T>
class ArrBuffer :public DynamicArr<T>{
public:
    ArrBuffer()noexcept :DynamicArr<T>(),data_size_(0){}
    ~ArrBuffer()noexcept {}

    ArrBuffer(const T* _data,usize _space)noexcept :DynamicArr<T>(_data,_space),data_size_(_space){}
    ArrBuffer(std::initializer_list<T> const& _ini_list) noexcept :DynamicArr<T>(_ini_list),data_size_(_ini_list.size()){}
    // move constructor
    ArrBuffer(ArrBuffer<T> && _other) noexcept :DynamicArr<T>(std::forward<ArrBuffer<T> &&>(_other)),data_size_(_other.data_size_){
        _other.data_size_ = 0;
    }

    ArrBuffer(DynamicArr<T>&& _other)noexcept :DynamicArr<T>(std::forward<DynamicArr<T>(_other)>),data_size_(_other.space_){}

    ArrBuffer& operator=(ArrBuffer<T> && _other) noexcept
    {
        this->DynamicArr<T>::operator=(std::forward<ArrBuffer<T> &&>(_other));
        this->data_size_ = _other.data_size_;
        return *this;
    }

    ArrBuffer<T> copy() const noexcept{
        ArrBuffer<T> res;
        if(this->space_){
            res.__space_grow(this->space_);
            memcpy(res.data_,this->data_,this->space_);
        }
        res.data_size_ = this->data_size_;
        return res.move();
    }

    ArrBuffer<T>&& move() noexcept override{
        return std::move(*this);
    }

    const usize size() const noexcept{
        return data_size_;
    }

    const usize capability() const noexcept{
        return this->space_;
    }

    void reserve_by(usize _size) noexcept{
        this->__space_grow(_size);
    }

    void reserve_to(usize _size) {
        if(this->space_ < _size){
            this->__space_grow(_size - this->space_);
        }else if(this->space_ > _size){
            this->__space_shrink(this->space_ - _size);
        }
    }

    DynamicArrIter<T> end(){
        return {*this,data_size_};
    }

    DynamicArrIterConst<T> end() const{
        return {*this,data_size_};
    }


    // push back single
    ArrBuffer& push_back(T const& _c) noexcept{
        usize ava_size =this->space_ - this->data_size_;
        if(ava_size){
            this->data_[this->data_size_] = _c;
            this->data_size_+=1;
        }else{
            usize grow_size = this->space_/2;
            this->__space_grow(grow_size);
            this->data_[this->data_size_] = _c;
            this->data_size_+=1;
        }
        return *this;
    }

    // push back multi
    ArrBuffer& push_back(ArrBuffer const& _another) noexcept{
        usize ava_size =this->space_ - this->data_size_;
        usize size_cost = _another.data_size_;
        if(ava_size<size_cost){
            this->__space_grow(size_cost);
        }
        memcpy(this->data_ + this->data_size_ ,_another.data_, size_cost);
        this->data_size_+=size_cost;
        return *this;
    }

    template<typename _InputType>
    requires( std::is_same_v<_InputType,ArrBuffer<T>> || std::is_base_of_v<ArrBuffer<byte>,T>) 
    ArrBuffer& operator+=(_InputType const& _in_data) noexcept{
        return this->push_back(_in_data);
    }

    template<typename _InputType>
    requires( std::is_same_v<_InputType,ArrBuffer<T>> || std::is_base_of_v<ArrBuffer<byte>,T>)
    ArrBuffer operator+(_InputType const& _in_data) noexcept{
        ArrBuffer res = this->copy();
        res.push_back(_in_data);
        return res.move();
    }

    // push front single
    ArrBuffer& push_front(T const& _c) noexcept{
        usize ava_size =this->space_ - this->data_size_;
        if(ava_size){
            memcpy(this->data_+1,this->data_,this->data_size_);
            this->data_[0] = _c;
            this->data_size_+=1;
        }else{
            usize grow_size = this->space_/2;
            this->__space_grow(grow_size);

            memcpy(this->data_+1,this->data_,this->data_size_);
            this->data_[0] = _c;
            this->data_size_+=1;
        }
        return *this;
    }

    // push front multi
    ArrBuffer& push_front(ArrBuffer const& _another) noexcept{
        usize ava_size =this->space_ - this->data_size_;
        usize size_cost = _another.data_size_;
        if(ava_size<size_cost){
            this->__space_grow(size_cost);
        }
        memcpy(this->data_ + size_cost,this->data_,this->data_size_);
        memcpy(this->data_,_another.data_, size_cost);
        this->data_size_+=size_cost;
        return *this;
    }
    
    // if enough read items and set them into _dist returns true
    // if not enough set as more as items into _dist returns false
    bool read_from_tail(T* _dist, usize _size) const {
        if(this->data_size_ >=_size){
            memcpy(_dist,this->data_ + (this->data_size_ - _size) , _size);
            return true;
        }else{
            memcpy(_dist,this->data_, this->data_size_);
            return false;
        }
    }

    bool erase_from_tail(usize _size) noexcept {
        if(this->data_size_ >=_size){
            this->data_size_ -= _size;
            return true;
        }
        else{
            this->data_size_ = 0;
            return false;
        }
    }

    // if enough read items and set them into _dist returns true
    // if not enough set as more as items into _dist returns false
    bool read_from_head(T* _dist, usize _size) const {
        if(this->data_size_ >=_size){
            memcpy(_dist,this->data_ , _size);
            return true;
        }else{
            memcpy(_dist,this->data_, this->data_size_);
            return false;
        }
    }

    bool erase_from_head(usize _size) noexcept {
        if(this->data_size_ >=_size){
            memcpy(this->data_,this->data_ +_size , this->data_size_ - _size);
            this->data_size_ -= _size;
            return true;
        }else{
            this->data_size_ = 0;
            return false;
        }
    }

    // reset the data_size_
    // but not set the data to 0
    void clear() noexcept {
        this->data_size_ = 0;
    }

    // clear all the data and shink to 0
    void zeromem() noexcept {
        this->data_size_ = 0;
        this->__shrink_to_zero();
    }

    DynamicArrIter<T> seek(usize _pos) noexcept {
        return {*this,_pos};
    }

    const T* view_and_move_forward(DynamicArrIter<T>& _iter, usize _size) {
        const T* res = this->data_ + _iter.pos_;
        if(_iter.pos_+_size >= this->data_size_){
            // out of range
            _iter.pos_ = this->data_size_;
        }else{
            _iter.pos_+=_size;
        }
        return res;
    }

    bool operator==(ArrBuffer const& _other) const noexcept{
        if(_other.size()!=this->size())return false;
        usize pos = 0;
        for(const auto&i:_other){
            if(i != this->operator[](pos)) return false;
            ++pos;
        }
        return true;
    }

protected:

    usize data_size_;

};

} // namespace tcx

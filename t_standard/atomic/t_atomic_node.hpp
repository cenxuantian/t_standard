#pragma once


namespace tcx
{

template<typename T>
struct AtomicHead{

};

template<typename T>
struct AtomicNode
{
    T* data_;
    AtomicNode* next_;
    AtomicNode* root_;
    AtomicNode* last_;
};
   
} // namespace tcx

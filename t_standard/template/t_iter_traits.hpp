#pragma once
#include "../basic/t_meta.h"

namespace tcx{

template<class C>
requires(__has_content_type_v<C>)
class TemplateIterTraits{
public:
    using OwnerType = C;
    using ContentType = C::ContentType;

    TemplateIterTraits(C& _owner):owner_(_owner){}
    virtual TemplateIterTraits& operator++() = 0;
    virtual C::ContentType& operator*() = 0;
    virtual bool operator!=(TemplateIterTraits<C> const&)=0;
protected:
    C& owner_;
};

template<class C>
requires(__has_content_type_v<C>)
class TemplateIterConstTraits{
public:
    using OwnerType = C;
    using ContentType = C::ContentType;
    
    TemplateIterConstTraits(C const& _owner):owner_(_owner){}

    virtual TemplateIterConstTraits& operator++() = 0;
    virtual C::ContentType const& operator*() = 0;
    virtual bool operator!=(TemplateIterConstTraits<C> const&)=0;
protected:
    C const& owner_;
};

template<typename CheckedTypes, typename OwnerType>
constexpr bool __is_iter_type_v = 
    std::is_base_of_v<TemplateIterTraits<OwnerType>,CheckedTypes> 
    || 
    std::is_base_of_v<TemplateIterConstTraits<OwnerType>,CheckedTypes>;

}
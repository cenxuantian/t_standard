#pragma once

namespace tcx
{

class NonMoveable{
public:
    NonMoveable(NonMoveable&&)=delete;
    NonMoveable& operator=(NonMoveable&&)=delete;
protected:
    NonMoveable()=default;
    ~NonMoveable()=default;
};

class NonCopyble{
public:
    NonCopyble(NonCopyble const&)=delete;
    NonCopyble& operator=(NonCopyble const&)=delete;

protected:
    NonCopyble()=default;
    ~NonCopyble()=default;
};



} // namespace tcx

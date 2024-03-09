#pragma once
#include "t_linklist.hpp"
namespace tcx
{
template<typename T>
using List = LinkList<DoubleNode<T>>;
} // namespace tcx

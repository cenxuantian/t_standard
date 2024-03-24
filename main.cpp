
#include <template/t_fixed_arr.hpp>
#include <template/t_arr_buffer.hpp>
#include <template/t_byte_buffer.hpp>
#include <template/t_string.hpp>
#include <template/t_byte_stream.hpp>
#include <template/t_linklist.hpp>
#include <template/t_queue.hpp>
#include <template/t_list.hpp>
#include <template/t_stack.hpp>
#include <template/t_red_black_tree.hpp>
#include <template/t_tree_map.hpp>
#include <utility/t_pair.hpp>
#include <utility/t_bit.hpp>
#include <algorithm/t_math.hpp>
#include <algorithm/t_sort.hpp>
#include <atomic/t_atomic_queue.hpp>
#include <atomic/t_atomic_ring_queue.hpp>
#include <template/t_search_binary_tree.hpp>

using namespace tcx;

int main(int, char**){
    // create the object
    TreeMap<std::string,int> m;
    m.emplace("b",10);
    m.emplace("a",100);

    m["a"] = 60;

    m.inc_traverse([](const auto& each){
        std::cout << each.first <<':' << each.second<<'\n';
    });

    return 0;

}

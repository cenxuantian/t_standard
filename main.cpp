
#include <template/t_fixed_arr.hpp>
#include <template/t_arr_buffer.hpp>
#include <template/t_byte_buffer.hpp>
#include <template/t_string.hpp>
#include <template/t_byte_stream.hpp>
#include <template/t_linklist.hpp>
#include <template/t_queue.hpp>
#include <template/t_list.hpp>
#include <template/t_stack.hpp>
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
    SearchBinaryTree<int> t;
    t.emplace(10);
    t.emplace(8);
    t.emplace(11);
    t.emplace(50);
    t.emplace(12);
    t.emplace(1);
    t.emplace(22);

    for(const auto & i : t){
        std::cout << i <<' ';
    }

    // t.post_traverse([](int const& each){
    //     std::cout << each<<'\n';
    // });

    return 0;

}

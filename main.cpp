
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
#include <algorithm/t_math.hpp>
#include <algorithm/t_sort.hpp>
#include <atomic/t_atomic_queue.hpp>
#include <atomic/t_atomic_ring_queue.hpp>

using namespace tcx;

int main(int, char**){
    // create the object
    AtomicRingQueue<int,100> q;
    // push something into the queue
    q.push(1);

    // try to pop
    auto pos = q.safe_pop_pos();
    // check if the pos is right
    if(pos!=0ULL){
        // read the data out
        int front = q.safe_pop(pos);
    }else{
        // the queue is empty currently
    }

    return 0;

}

#pragma once
#include "../utility/t_bit.hpp"

namespace tcx
{
namespace websocket
{
enum class Opcode: unsigned char{
    CONTINUE_FRAME  = 0x00,
    TEXT_FRAME      = 0x01,
    BINARY_FRAME    = 0x02,
    RSV_NON_CTRL_1 = 0x03,
    RSV_NON_CTRL_2 = 0x04,
    RSV_NON_CTRL_3 = 0x05,
    RSV_NON_CTRL_4 = 0x06,
    RSV_NON_CTRL_5 = 0x07,
    CONNECTION_CLOSE = 0x08,
    PING_FRAME = 0x09,
    PONG_FRAME = 0x0A,
    RSV_CTRL_1 = 0x0B,
    RSV_CTRL_2 = 0x0C,
    RSV_CTRL_3 = 0x0D,
    RSV_CTRL_4 = 0x0E,
    RSV_CTRL_5 = 0x0F,
};

struct __FixedHead{
    int FIN:1;
    int RSV1:1;
    int RSV2:1;
    int RSV3:1;
    int opcode:4;
    int mask:1;
    int payload_len:7;
};

union FixedHead{
    u16 disp;
    __FixedHead bits;
};


struct Frame{
    FixedHead fixed_head{0};
    u64 real_payload_len;
    bit32_t mask_key{0};
    
};

} // namespace websocket

} // namespace tcx

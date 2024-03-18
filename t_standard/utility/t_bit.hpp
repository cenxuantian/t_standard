#pragma once
#include "../basic/t_int.h"
#include "../basic/t_float.h"

namespace tcx
{

struct __bit8_t{
    int _0 :1 = 0;
    int _1 :1 = 0;
    int _2 :1 = 0;
    int _3 :1 = 0;
    int _4 :1 = 0;
    int _5 :1 = 0;
    int _6 :1 = 0;
    int _7 :1 = 0;
};

struct __bit16_t{
    int _0 :1 = 0;
    int _1 :1 = 0;
    int _2 :1 = 0;
    int _3 :1 = 0;
    int _4 :1 = 0;
    int _5 :1 = 0;
    int _6 :1 = 0;
    int _7 :1 = 0;
    int _8 :1 = 0;
    int _9 :1 = 0;
    int _a :1 = 0;
    int _b :1 = 0;
    int _c :1 = 0;
    int _d :1 = 0;
    int _e :1 = 0;
    int _f :1 = 0;

};

struct __bit32_t{
    int _0 :1 = 0;
    int _1 :1 = 0;
    int _2 :1 = 0;
    int _3 :1 = 0;
    int _4 :1 = 0;
    int _5 :1 = 0;
    int _6 :1 = 0;
    int _7 :1 = 0;
    int _8 :1 = 0;
    int _9 :1 = 0;
    int _a :1 = 0;
    int _b :1 = 0;
    int _c :1 = 0;
    int _d :1 = 0;
    int _e :1 = 0;
    int _f :1 = 0;
    int _10 :1 = 0;
    int _11 :1 = 0;
    int _12 :1 = 0;
    int _13 :1 = 0;
    int _14 :1 = 0;
    int _15 :1 = 0;
    int _16 :1 = 0;
    int _17 :1 = 0;
    int _18 :1 = 0;
    int _19 :1 = 0;
    int _1a :1 = 0;
    int _1b :1 = 0;
    int _1c :1 = 0;
    int _1d :1 = 0;
    int _1e :1 = 0;
    int _1f :1 = 0;

};

struct __bit64_t{
    int _0 :1 = 0;
    int _1 :1 = 0;
    int _2 :1 = 0;
    int _3 :1 = 0;
    int _4 :1 = 0;
    int _5 :1 = 0;
    int _6 :1 = 0;
    int _7 :1 = 0;
    int _8 :1 = 0;
    int _9 :1 = 0;
    int _a :1 = 0;
    int _b :1 = 0;
    int _c :1 = 0;
    int _d :1 = 0;
    int _e :1 = 0;
    int _f :1 = 0;
    int _10 :1 = 0;
    int _11 :1 = 0;
    int _12 :1 = 0;
    int _13 :1 = 0;
    int _14 :1 = 0;
    int _15 :1 = 0;
    int _16 :1 = 0;
    int _17 :1 = 0;
    int _18 :1 = 0;
    int _19 :1 = 0;
    int _1a :1 = 0;
    int _1b :1 = 0;
    int _1c :1 = 0;
    int _1d :1 = 0;
    int _1e :1 = 0;
    int _1f :1 = 0;
    int _21 :1 = 0;
    int _22 :1 = 0;
    int _23 :1 = 0;
    int _24 :1 = 0;
    int _25 :1 = 0;
    int _26 :1 = 0;
    int _27 :1 = 0;
    int _28 :1 = 0;
    int _29 :1 = 0;
    int _2a :1 = 0;
    int _2b :1 = 0;
    int _2c :1 = 0;
    int _2d :1 = 0;
    int _2e :1 = 0;
    int _2f :1 = 0;
    int _31 :1 = 0;
    int _32 :1 = 0;
    int _33 :1 = 0;
    int _34 :1 = 0;
    int _35 :1 = 0;
    int _36 :1 = 0;
    int _37 :1 = 0;
    int _38 :1 = 0;
    int _39 :1 = 0;
    int _3a :1 = 0;
    int _3b :1 = 0;
    int _3c :1 = 0;
    int _3d :1 = 0;
    int _3e :1 = 0;
    int _3f :1 = 0;

};


union bit8_t{
    u8 disp;
    __bit8_t bits;
};

union bit16_t{
    u16 disp;
    __bit16_t bits;
};

union bit32_t{
    u32 disp;
    __bit32_t bits;
};

union bit64_t{
    u64 disp;
    __bit64_t bits;
};

union bit32f_t{
    f32 disp;
    __bit32_t bits;
};

union bit64f_t{
    f64 disp;
    __bit32_t bits;
};


} // namespace tcx

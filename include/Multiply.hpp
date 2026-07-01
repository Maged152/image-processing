#pragma once

#include "common/types.hpp"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
    Image<frmt, TO> Multiply(
        const Image<frmt, T1> &in1,
        const Image<frmt, T2> &in2,
        const double scale = 1.0,
        const OverFlowFlag &over_flow = OverFlowFlag::SATURATE
    );
}
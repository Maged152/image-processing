#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<pixel_t T>
	Image<ImageFormat::GRAY, std::make_unsigned_t<T>> Magnitude(const Image<ImageFormat::GRAY, T>& g_x,
                                                                const Image<ImageFormat::GRAY, T>& g_y,
                                                                const bool L2gradient = false);
} 
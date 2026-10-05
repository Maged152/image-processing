#pragma once

#include "common/types.hpp"
#include <type_traits>

namespace qlm
{
	template<pixel_t T, pixel_t TO>
	void Magnitude(const Image<ImageFormat::GRAY, T>& g_x,
	               const Image<ImageFormat::GRAY, T>& g_y,
	               Image<ImageFormat::GRAY, TO>& out,
	               const bool L2gradient = false,
	               const Rectangle<int>& roi = Rectangle<int>{});

	template<pixel_t T, pixel_t TO = std::make_unsigned_t<T>>
	Image<ImageFormat::GRAY, TO> Magnitude(const Image<ImageFormat::GRAY, T>& g_x,
	                                       const Image<ImageFormat::GRAY, T>& g_y,
	                                       const bool L2gradient = false,
	                                       const Rectangle<int>& roi = Rectangle<int>{});
} 
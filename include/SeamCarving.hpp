#pragma once
#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(
		const Image<frmt, T>& in,
		const int width,
		const int height,
		const EnergyFlag energy = EnergyFlag::BACKWARD,
		const OrderFlag order = OrderFlag::WIDTH_FIRST);

}
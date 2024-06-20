#pragma once
#include "common/types.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(
		const Image<frmt, T>& in,
		const size_t width,
		const size_t height,
		const EnergyFlag energy = EnergyFlag::BACKWARD,
		const OrderFlag order = OrderFlag::WIDTH_FIRST);

}
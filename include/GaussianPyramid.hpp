#pragma once

#include "common/types.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Pyramid<frmt, T> GaussianPyramid(
		const Image<frmt, T>& in,
        const size_t num_layers,
		const float scale = 0.5f,
		const size_t filter_size = 5,
		const float sigma = 1.0f,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);

}
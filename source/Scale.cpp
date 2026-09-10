#include "Scale.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Scale(const qlm::Image<frmt, T>& in,
						       const qlm::InterpolationFlag inter, const float scale_x, const float scale_y,
							   const BorderMode<frmt, T>& border_mode)
{
	const int width = in.width;
	const int height = in.height;

	// create image with the new size
	const int new_width = std::round(width * scale_x);
	const int new_height = std::round(height * scale_y);

	// invalid scale (zero or negative output dimensions)
	if (new_width <= 0 || new_height <= 0)
	{
		return qlm::Image<frmt, T>{};
	}

	qlm::Image<frmt, T> out;
	out.Create(new_width, new_height, border_mode.border_pixel);

	for (int x = 0; x < new_width; x++)
	{
		for (int y = 0; y < new_height; y++)
		{
			// value from input image
			float in_x = x / scale_x;
			float in_y = y / scale_y;
			// check if inside the image
			if (in_x < 0 || in_y < 0 || in_x >= in.width || in_y >= in.height)
				continue;
			// do interpolation
			const auto pix = qlm::Interpolation(in, in_x, in_y, inter, border_mode);
			// store pixel
			out.SetPixel(x, y, pix);

		}
	}

	return out;
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t> 
qlm::Scale<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
										   const qlm::InterpolationFlag,
										   const float,
										   const float,
										   const BorderMode<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Scale<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
											const qlm::InterpolationFlag,
											const float,
											const float,
											const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);


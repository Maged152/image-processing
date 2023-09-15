#include "WarpAffine/WarpAffine.h"
#include <utility>
#include <iostream>
#include <algorithm>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(const Image<frmt, T>& in, const TransformationMatrix& mat, const int dst_width, const int dst_height, const InterpolationFlag inter, const BorderMode<frmt, T>& border_mode)
	{
		// create the output image
		Image<frmt, T> out;
		out.create(dst_width, dst_height,border_mode.border_pixel);
		// do transformation
		auto transform_out_in = [&](int out_x, int out_y)
		{
			float c = out_x - mat.GetElement(0, 2);
			float f = out_y - mat.GetElement(1, 2);

			float den = mat.GetElement(0, 0) * mat.GetElement(1, 1) - mat.GetElement(0, 1) * mat.GetElement(1, 0);

			float in_x = (c * mat.GetElement(1, 1) - mat.GetElement(0, 1) * f) / den;
			float in_y = (f * mat.GetElement(0, 0) - mat.GetElement(1, 0) * c) / den;

			return std::pair<float, float>(in_x, in_y);
		};

		auto transform_in_out = [&](int in_x, int in_y)
		{
			float out_x = in_x * mat.GetElement(0, 0) + in_y * mat.GetElement(0, 1) + mat.GetElement(0, 2);
			float out_y = in_x * mat.GetElement(1, 0) + in_y * mat.GetElement(1, 1) + mat.GetElement(1, 2);

			return std::pair<float, float>(out_x, out_y);
		};
		
		// start/end index
		const auto [tl_x, tl_y] = transform_in_out(0, 0);
		const auto [tr_x, tr_y] = transform_in_out(in.Width() - 1, 0);
		const auto [br_x, br_y] = transform_in_out(in.Width() - 1, in.Height() -1);
		const auto [bl_x, bl_y] = transform_in_out(0, in.Height() - 1);
		
		int start_x = std::min(std::min(tl_x, tr_x), std::min(br_x, bl_x));
		int end_x   = std::max(std::max(tl_x, tr_x), std::max(br_x, bl_x));

		int start_y = std::min(std::min(tl_y, tr_y), std::min(br_y, bl_y));
		int end_y   = std::max(std::max(tl_y, tr_y), std::max(br_y, bl_y));
		
		// loop over the output image
		for (int y = start_y; y < end_y; y++)
		{
			for (int x = start_x; x < end_x; x++)
			{
				// do transformation
				const auto [in_x, in_y] = transform_out_in(x, y);
				// check if inside the image
				if (in_x < 0 || in_y < 0 || in_x >= in.Width() || in_y >= in.Height())
					continue;
				// get pixel
				const auto pix = Interpolation(in, in_x, in_y, inter, border_mode);
				// store pixel
				out.SetPixel(x, y, pix);
			}
		}
		return out;
	}

	template Image<ImageFormat::RGB, uint8_t> WarpAffine(const Image<ImageFormat::RGB, uint8_t >&,
				const TransformationMatrix&, const int, const int, 
				const InterpolationFlag, const BorderMode<ImageFormat::RGB, uint8_t>&);


	template Image<ImageFormat::GRAY, uint8_t> WarpAffine(const Image<ImageFormat::GRAY, uint8_t >&,
		const TransformationMatrix&, const int, const int,
		const InterpolationFlag, const BorderMode<ImageFormat::GRAY, uint8_t>&);

}
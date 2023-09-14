#include "WarpAffine/WarpAffine.h"
#include <utility>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(const Image<frmt, T>& in, const TransformationMatrix& mat, const int dst_width, const int dst_height, const InterpolationFlag inter, const BorderMode<frmt, T>& border_mode)
	{
		// create the output image
		Image<frmt, T> out;
		out.create(dst_width, dst_height,border_mode.border_pixel);
		// start/end index for input image
		const int in_start_x = 0; const int in_start_y = 0;
		const int in_end_x = in.Width(); const int in_end_y = in.Height();
		// start/end index for output image
		const int out_start_x = in_start_x * mat.GetElement(0, 0) + in_start_y * mat.GetElement(0, 1) + mat.GetElement(0, 2);
		const int out_end_x = in_end_x * mat.GetElement(0, 0) + in_end_y * mat.GetElement(0, 1) + mat.GetElement(0, 2);

		const int out_start_y = in_start_x * mat.GetElement(1, 0) + in_start_y * mat.GetElement(1, 1) + mat.GetElement(1, 2);
		const int out_end_y = in_end_x * mat.GetElement(1, 0) + in_end_y * mat.GetElement(1, 1) + mat.GetElement(1, 2);
		// do transformation
		auto transform = [&](int out_x, int out_y)
		{
			float c = out_x - mat.GetElement(0, 2);
			float f = out_y - mat.GetElement(1, 2);

			float den = mat.GetElement(0, 0) * mat.GetElement(1, 1) - mat.GetElement(0, 1) * mat.GetElement(1, 0);

			float in_x = (c * mat.GetElement(1, 1) - mat.GetElement(0, 1) * f) / den;
			float in_y = (f * mat.GetElement(0, 0) - mat.GetElement(1, 0) * c) / den;

			return std::pair<float, float>(in_x, in_y);
		};
		// loop over the output image
		for (int y = out_start_y; y < out_end_y; y++)
		{
			for (int x = out_start_x; x < out_end_x; x++)
			{
				// do transformation
				const auto [in_x, in_y] = transform(x, y);
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
	
}
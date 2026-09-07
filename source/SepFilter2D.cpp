#include "SepFilter2D.hpp"
#include <cassert>

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	void SepFilter2D(const Image<frmt, src_t>& in,
		Image<frmt, dst_t>& out,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode,
		const Rectangle<int>& roi)
	{
		const int img_width = in.width;
		const int img_height = in.height;

		// validate that the output has the same dimensions as the input
		assert(in.width == out.width && in.height == out.height);

		auto [start_x, start_y, end_x, end_y] = ROI(roi, img_width, img_height);

		int x_padding_length = kernel.x_ker.Length() / 2;
		int y_padding_length = kernel.y_ker.Length() / 2;

		Pixel<frmt, float> weight_sum;

		// temp_pixel_array to hold y_kernel values
		Image<frmt, float> temp_y_filter;
		temp_y_filter.Create(img_width, 1);

		auto border_mode_f = BorderMode<frmt, float>{};
		border_mode_f.border_type = border_mode.border_type;
		border_mode_f.border_pixel = border_mode.border_pixel;

		// the x-kernel pass reads the y-filtered values in a window extended
		// by the x padding, so the y-kernel pass only needs those columns
		const int temp_start_x = start_x - x_padding_length > 0 ? start_x - x_padding_length : 0;
		const int temp_end_x = end_x + x_padding_length < img_width ? end_x + x_padding_length : img_width;

		for (int y = start_y; y < end_y; y++)
		{
			// y-kernel (only the columns needed by the x-kernel pass)
			for (int x = temp_start_x; x < temp_end_x; x++)
			{
				// Reset the weight_sum array
				weight_sum.Set(0.0f);

				for (int i = -y_padding_length; i <= y_padding_length; i++)
				{
					// get the pixel
					const auto in_pixel = in.GetPixel(x, y + i, border_mode);
					auto co = kernel.y_ker.Get(i + y_padding_length);
					weight_sum.MAC(in_pixel, co);
				}
				// store the output
				temp_y_filter.SetPixel(x, weight_sum);
			}
			// x-kernel (working area only)
			for (int x = start_x; x < end_x; x++)
			{
				// Reset
				weight_sum.Set(0.0f);

				for (int i = -x_padding_length; i <= x_padding_length; i++)
				{
					// get the pixel
					const auto in_pixel = temp_y_filter.GetPixel(x + i, 0, border_mode_f);

					weight_sum.MAC(in_pixel, kernel.x_ker.Get(i + x_padding_length));
				}
				// store the output
				out.SetPixel(x, y, weight_sum);
			}
		}
	}

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(const Image<frmt, src_t>& in,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode,
		const Rectangle<int>& roi)
	{
		// create the output image
		Image<frmt, dst_t> out (in.width, in.height);

		SepFilter2D(in, out, kernel, border_mode, roi);

		return out;
	}


	// Explicit instantiation for RGB , uint8_t , uint8_t
	template Image<ImageFormat::RGB, uint8_t>
		SepFilter2D<ImageFormat::RGB, uint8_t, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::RGB, uint8_t, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
			Image<ImageFormat::RGB, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&,
			const Rectangle<int>&);
	// Explicit instantiation for RGB , uint8_t, int16_t
	template Image<ImageFormat::RGB, int16_t>
		SepFilter2D<ImageFormat::RGB, uint8_t, int16_t>(const Image<ImageFormat::RGB, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::RGB, uint8_t, int16_t>(const Image<ImageFormat::RGB, uint8_t>&,
			Image<ImageFormat::RGB, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&,
			const Rectangle<int>&);
	// Explicit instantiation for RGB , int16_t, int16_t
	template Image<ImageFormat::RGB, int16_t>
		SepFilter2D<ImageFormat::RGB, int16_t, int16_t>(const Image<ImageFormat::RGB, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, int16_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::RGB, int16_t, int16_t>(const Image<ImageFormat::RGB, int16_t>&,
			Image<ImageFormat::RGB, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, int16_t>&,
			const Rectangle<int>&);
	// Explicit instantiation for GRAY , uint8_t, int16_t
	template Image<ImageFormat::GRAY, int16_t>
		SepFilter2D<ImageFormat::GRAY, uint8_t, int16_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::GRAY, uint8_t, int16_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			Image<ImageFormat::GRAY, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&,
			const Rectangle<int>&);
	// Explicit instantiation for GRAY , int16_t, int16_t
	template Image<ImageFormat::GRAY, int16_t>
		SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(const Image<ImageFormat::GRAY, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, int16_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(const Image<ImageFormat::GRAY, int16_t>&,
			Image<ImageFormat::GRAY, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, int16_t>&,
			const Rectangle<int>&);
	// Explicit instantiation for GRAY , uint8_t, uint8_t
	template Image<ImageFormat::GRAY, uint8_t>
		SepFilter2D<ImageFormat::GRAY, uint8_t, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::GRAY, uint8_t, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			Image<ImageFormat::GRAY, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&,
			const Rectangle<int>&);
		// Explicit instantiation for GRAY , uint8_t, float
	template Image<ImageFormat::GRAY, float>
		SepFilter2D<ImageFormat::GRAY, float, float>(const Image<ImageFormat::GRAY, float>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, float>&,
			const Rectangle<int>&);
	template void
		SepFilter2D<ImageFormat::GRAY, float, float>(const Image<ImageFormat::GRAY, float>&,
			Image<ImageFormat::GRAY, float>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, float>&,
			const Rectangle<int>&);

}
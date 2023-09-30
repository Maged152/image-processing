#include "SepFilter2D/SepFilter2D.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(const Image<frmt, src_t>& in,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode)
	{
		unsigned int img_width = in.Width();
		unsigned int img_height = in.Height();

		// create the output image
		Image<frmt, dst_t> out;
		out.create(img_width, img_height);

		int x_padding_length = kernel.x_ker.Length() / 2;
		int y_padding_length = kernel.y_ker.Length() / 2;

		Pixel<frmt, float> weight_sum;

		// temp_pixel_array to hold y_kernel values
		Image<frmt, float> temp_y_filter;
		temp_y_filter.create(img_width, 1);

		auto border_mode_f = BorderMode<frmt, float>{};
		border_mode_f.border_type = border_mode.border_type;
		border_mode_f.border_pixel = border_mode.border_pixel;

		for (int y = 0; y < img_height; y++)
		{
			// y-kernel
			for (int x = 0; x < img_width; x++)
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
			// x-kernel
			for (int x = 0; x < img_width; x++)
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

		return out;
	}


	// Explicit instantiation for RGB , uint8_t , uint8_t
	template Image<ImageFormat::RGB, uint8_t>
		SepFilter2D<ImageFormat::RGB, uint8_t, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&);
	// Explicit instantiation for RGB , uint8_t, int16_t
	template Image<ImageFormat::RGB, int16_t>
		SepFilter2D<ImageFormat::RGB, uint8_t, int16_t>(const Image<ImageFormat::RGB, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, uint8_t>&);
	// Explicit instantiation for RGB , int16_t, int16_t
	template Image<ImageFormat::RGB, int16_t>
		SepFilter2D<ImageFormat::RGB, int16_t, int16_t>(const Image<ImageFormat::RGB, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::RGB, int16_t>&);
	// Explicit instantiation for GRAY , uint8_t, int16_t
	template Image<ImageFormat::GRAY, int16_t>
		SepFilter2D<ImageFormat::GRAY, uint8_t, int16_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&);
	// Explicit instantiation for GRAY , int16_t, int16_t
	template Image<ImageFormat::GRAY, int16_t>
		SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(const Image<ImageFormat::GRAY, int16_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, int16_t>&);
	// Explicit instantiation for GRAY , uint8_t, uint8_t
	template Image<ImageFormat::GRAY, uint8_t>
		SepFilter2D<ImageFormat::GRAY, uint8_t, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&,
			const SepKernel&,
			const BorderMode<ImageFormat::GRAY, uint8_t>&);

}
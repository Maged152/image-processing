#include "Filter2D/Filter2D.h"

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Filter2D(const qlm::Image<frmt, T>& in, const qlm::Kernel& kernel, const  BorderMode<frmt, T>& border_mode)
{
	unsigned int img_width = in.Width();
	unsigned int img_height = in.Height();

	// create the output image
	qlm::Image<frmt, T> out;
	out.create(img_width, img_height, qlm::Pixel<frmt, T>{});

	int pad_width = kernel.width / 2;
	int pad_height = kernel.height / 2;

	qlm::Pixel<frmt, T> in_pixel;
	qlm::Pixel<frmt, float> weight_sum;

	for (int y = 0; y < img_height; y++)
	{
		for (int x = 0; x < img_width; x++)
		{
			// Reset the sum array for each pixel
			weight_sum.Set(0.0f);

			for (int j = -pad_height; j <= pad_height; j++)
			{
				for (int i = -pad_width; i <= pad_width; i++)
				{
					// get the pixel
					int x_idx = x + i;
					int y_idx = y + j;

					in_pixel = in.GetPixel(x_idx, y_idx, border_mode);
					
					weight_sum.MAC(in_pixel, kernel.Get(i + pad_width, j + pad_height));
				}
			}
			// store the output
			out.SetPixel(x, y, weight_sum);
		}
	}

	return out;
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::Filter2D<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
											  const qlm::Kernel& ,
											  const BorderMode<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::Filter2D<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	                                          const qlm::Kernel&,
											  const BorderMode<qlm::ImageFormat::RGB, int16_t>&);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Filter2D<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	                                           const qlm::Kernel&,
											   const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);
// Explicit instantiation for GRAY , int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::Filter2D<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	                                           const qlm::Kernel&,
											   const BorderMode<qlm::ImageFormat::GRAY, int16_t>&);

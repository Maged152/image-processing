#include "SepFilter2D/SepFilter2D.h"
#include <iostream>

static inline int ReflectBorderIndex(int idx, int max_idx)
{
	int reflect_idx = idx;
	if (idx < 0)
	{
		reflect_idx = -idx - 1;
	}
	else if (idx >= max_idx)
	{
		reflect_idx = max_idx - (idx - max_idx) - 1;
	}
	return reflect_idx;
}

template<qlm::ImageFormat frmt, qlm::pixel_t src_t, qlm::pixel_t dst_t>
qlm::Image<frmt, dst_t> qlm::SepFilter2D(const qlm::Image<frmt, src_t>& in,
	                       const qlm::Kernel1D& x_kernel, const qlm::Kernel1D& y_kernel, 
	                       const qlm::Border border_type, const int border_value)
{
	unsigned int img_width = in.Width();
	unsigned int img_height = in.Height();

	// create the output image
	qlm::Image<frmt, dst_t> out;
	out.create(img_width, img_height, qlm::Pixel<frmt, dst_t>{});

	int x_padding_length = x_kernel.length / 2;
	int y_padding_length = y_kernel.length / 2;

	// for now float until the qlm::kernel will be template
	qlm::Pixel<frmt, float> pixel_filter;
	qlm::Pixel<frmt, float> weight_sum;
	qlm::Pixel<frmt, float> temp_pixel;

	// temp_pixel_array to hold y_kernel values 
	auto temp_pixel_array = new qlm::Pixel<frmt, float>[img_width];

	// get the pixel from the input image and put it in pixel_filter
	auto get_pixel_at = [&](int x_idx, int y_idx)
	{
		if (x_idx >= 0 && x_idx < img_width && y_idx >= 0 && y_idx < img_height)
		{
			pixel_filter = in.GetPixel(x_idx, y_idx);
		}
		else
		{
			switch (border_type)
			{
				case qlm::Border::BORDER_CONSTANT:
					{
						pixel_filter.Set(border_value);
						break;
					}
				case qlm::Border::BORDER_REPLICATE:
					{
						x_idx = std::clamp(x_idx, 0, static_cast<int>(img_width) - 1);
						y_idx = std::clamp(y_idx, 0, static_cast<int>(img_height) - 1);
						pixel_filter = in.GetPixel(x_idx, y_idx);
						break;
					}
				case qlm::Border::BORDER_REFLECT:
					{
						x_idx = ReflectBorderIndex(x_idx, img_width);
						y_idx = ReflectBorderIndex(y_idx, img_height);
						pixel_filter = in.GetPixel(x_idx, y_idx);
						break;
					}
			}
		}

	};

	// get the pixel from the temp array of pixels and put it in temp_pixel
	auto get_temp_pixel_at = [&](int x_idx)
	{
		if (x_idx >= 0 && x_idx < img_width)
		{
			temp_pixel = temp_pixel_array[x_idx];
		}
		else
		{
			switch (border_type)
			{
				case qlm::Border::BORDER_CONSTANT:
				{
					temp_pixel.Set(border_value);
					break;
				}
				case qlm::Border::BORDER_REPLICATE:
				{
					x_idx = std::clamp(x_idx, 0, static_cast<int>(img_width) - 1);
					temp_pixel = temp_pixel_array[x_idx];
					break;
				}
				case qlm::Border::BORDER_REFLECT:
				{
					x_idx = ReflectBorderIndex(x_idx, img_width);
					temp_pixel = temp_pixel_array[x_idx];
					break;
				}
			}
		}

	};
	
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
				get_pixel_at(x, y + i); // Pixel in "pixel_filter"

				weight_sum.MAC(pixel_filter, y_kernel.Get(i + y_padding_length));
			}
			// store the output
			temp_pixel_array[x] = weight_sum;
		}
		// x-kernel
		for (int x = 0; x < img_width; x++)
		{
			// Reset
			weight_sum.Set(0.0f);
			
			for (int i = -x_padding_length; i <= x_padding_length; i++)
			{
				// get the pixel
				get_temp_pixel_at(x + i); // Pixel in "temp_pixel"
				// use temp_pixel_array array
				weight_sum.MAC(temp_pixel, x_kernel.Get(i + x_padding_length));
			}
			// store the output
			out.SetPixel(x, y, weight_sum);
		}
	}

	return out;
}


// Explicit instantiation for RGB , uint8_t , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);
// Explicit instantiation for RGB , uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);
// Explicit instantiation for RGB , int16_t, int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::SepFilter2D<qlm::ImageFormat::RGB, int16_t, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);
// Explicit instantiation for GRAY , uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::SepFilter2D<qlm::ImageFormat::GRAY, uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);
// Explicit instantiation for GRAY , int16_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::SepFilter2D<qlm::ImageFormat::GRAY, int16_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);
// Explicit instantiation for GRAY , uint8_t, uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::SepFilter2D<qlm::ImageFormat::GRAY, uint8_t, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const qlm::Kernel1D&,
	const qlm::Kernel1D&,
	const qlm::Border,
	const int);


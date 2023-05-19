#include "Filter2D/Filter2D.h"
#include <algorithm>
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

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Filter2D(const qlm::Image<frmt, T>& in, const qlm::Kernel& kernel, qlm::Border border_type, int border_value)
{
	unsigned int img_width = in.Width();
	unsigned int img_height = in.Height();

	// create the output image
	qlm::Image<frmt, T> out;
	out.create(img_width, img_height, qlm::Pixel<frmt, T>{});

	int pad_width = kernel.width / 2;
	int pad_height = kernel.height / 2;

	// for now float until the qlm::kernel will be template
	qlm::Pixel<frmt, float> pixel_filter;
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
					
					weight_sum.MAC(pixel_filter, kernel.Get(i + pad_width, j + pad_height));
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
											  qlm::Border,
											  int);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::Filter2D<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	                                          const qlm::Kernel&,
	                                          qlm::Border,
	                                          int);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Filter2D<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	                                           const qlm::Kernel&,
	                                           qlm::Border,
	                                           int);
// Explicit instantiation for GRAY , int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::Filter2D<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	                                           const qlm::Kernel&,
	                                           qlm::Border,
	                                           int);

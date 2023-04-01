#include "SepFilter2D.h"
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


void qlm::SepFilter2D(const sf::Image& in, sf::Image& out, 
	                  const qlm::Kernel1D& x_kernel, const qlm::Kernel1D& y_kernel, 
	                  const qlm::BORDER border_type, const int border_value)
{
	if (in.getSize().x != out.getSize().x || in.getSize().y != out.getSize().y)
	{
		std::cout << "Input and output images must have the same size!\n";
		return;
	}
	
	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;

	int x_padding_length = x_kernel.length / 2;
	int y_padding_length = y_kernel.length / 2;

	sf::Color pixel_result;
	qlm::Pixel temp_pixel;  
	qlm::Pixel pixel_sum;

	// temp_pixel_array to hold y_kernel values 
	auto temp_pixel_array = new qlm::Pixel[img_width];

	// get the pixel from the input image and put it in pixel_result
	auto get_pixel_at = [&](int x_idx, int y_idx)
	{
		if (x_idx >= 0 && x_idx < img_width && y_idx >= 0 && y_idx < img_height)
		{
			pixel_result = in.getPixel(x_idx, y_idx);
		}
		else
		{
			switch (border_type)
			{
				case qlm::BORDER::BORDER_CONSTANT:
					{
						pixel_result.r = border_value;
						pixel_result.g = border_value;
						pixel_result.b = border_value;
						pixel_result.a = border_value;
						break;
					}
				case qlm::BORDER::BORDER_REPLICATE:
					{
						x_idx = std::clamp(x_idx, 0, static_cast<int>(img_width) - 1);
						y_idx = std::clamp(y_idx, 0, static_cast<int>(img_height) - 1);
						pixel_result = in.getPixel(x_idx, y_idx);
						break;
					}
				case qlm::BORDER::BORDER_REFLECT:
					{
						x_idx = ReflectBorderIndex(x_idx, img_width);
						y_idx = ReflectBorderIndex(y_idx, img_height);
						pixel_result = in.getPixel(x_idx, y_idx);
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
			case qlm::BORDER::BORDER_CONSTANT:
			{
				temp_pixel.r = border_value;
				temp_pixel.g = border_value;
				temp_pixel.b = border_value;
				temp_pixel.a = border_value;
				break;
			}
			case qlm::BORDER::BORDER_REPLICATE:
			{
				x_idx = std::clamp(x_idx, 0, static_cast<int>(img_width) - 1);
				temp_pixel = temp_pixel_array[x_idx];
				break;
			}
			case qlm::BORDER::BORDER_REFLECT:
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
			// Reset the pixel_sum array
			pixel_sum.Set(0.0f);
			
			for (int i = -y_padding_length; i <= y_padding_length; i++)
			{
				// get the pixel
				get_pixel_at(x, y + i); // Pixel in "pixel_result"

				pixel_sum.MAC(pixel_result, y_kernel.Get(i + y_padding_length));
			}
			// store the output
			temp_pixel_array[x] = pixel_sum;
		}
		// x-kernel
		for (int x = 0; x < img_width; x++)
		{
			// Reset
			pixel_sum.Set(0.0f);
			
			for (int i = -x_padding_length; i <= x_padding_length; i++)
			{
				// get the pixel
				get_temp_pixel_at(x + i); // Pixel in "temp_pixel"
				// use temp_pixel_array array
				pixel_sum.MAC(temp_pixel, x_kernel.Get(i + x_padding_length));
			}
			// store the output
			pixel_sum.ToColor(pixel_result);
			out.setPixel(x, y, pixel_result);
		}
	}
}

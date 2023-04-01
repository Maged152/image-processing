#include "Filter2D.h"
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

void qlm::Filter2D(const sf::Image& in, sf::Image& out, const qlm::Kernel& kernel, qlm::BORDER border_type, int border_value)
{
	if (in.getSize().x != out.getSize().x || in.getSize().y != out.getSize().y)
	{
		std::cout << "Input and output images must have the same size!\n";
		return;
	}

	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;

	int pad_width = kernel.width / 2;
	int pad_height = kernel.height / 2;

	sf::Color pixel_result;
	qlm::Pixel pixel_sum;


	for (int y = 0; y < img_height; y++)
	{
		for (int x = 0; x < img_width; x++)
		{
			// Reset the sum array for each pixel
			pixel_sum.Set(0.0f);

			for (int j = -pad_height; j <= pad_height; j++)
			{
				for (int i = -pad_width; i <= pad_width; i++)
				{
					// get the pixel
					int x_idx = x + i;
					int y_idx = y + j;
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
					
					pixel_sum.MAC(pixel_result, kernel.Get(i + pad_width, j + pad_height));
				}
			}
			// store the output
			pixel_sum.ToColor(pixel_result);
			out.setPixel(x, y, pixel_result);
		}
	}
}

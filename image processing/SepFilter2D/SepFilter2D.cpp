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
static inline void MAC(float sum[4], sf::Color& c, float coeff)
{
	sum[0] += static_cast<float>(c.r) * coeff;
	sum[1] += static_cast<float>(c.g) * coeff;
	sum[2] += static_cast<float>(c.b) * coeff;
	sum[3] += static_cast<float>(c.a) * coeff;
}

static inline void StorePix(float sum[4], sf::Color& c)
{
	c.r = std::clamp(static_cast<int>(sum[0]), 0, 255);
	c.g = std::clamp(static_cast<int>(sum[1]), 0, 255);
	c.b = std::clamp(static_cast<int>(sum[2]), 0, 255);
	c.a = std::clamp(static_cast<int>(sum[3]), 0, 255);
}


void qlm::SepFilter2D(const sf::Image& in, sf::Image& out, 
	                  const qlm::Kernel1D& kernel_x, const qlm::Kernel1D& kernel_y, 
	                  const qlm::BORDER border, const int border_const)
{
	if (in.getSize().x != out.getSize().x || in.getSize().y != out.getSize().y)
	{
		std::cout << "Input and output images must have the same size!\n";
		return;
	}
	
	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;

	int pad_len_x = kernel_x.length / 2;
	int pad_len_y = kernel_y.length / 2;

	sf::Color res;
	float sum[4] = { 0 };
	// TODO 
	// array to hold res of y_ kernel to be used in x_kernel
	auto GetPixel = [&](int x_idx, int y_idx)
	{
		if (x_idx >= 0 && x_idx < img_width && y_idx >= 0 && y_idx < img_height)
		{
			res = in.getPixel(x_idx, y_idx);
		}
		else
		{
			switch (border)
			{
				case qlm::BORDER::BORDER_CONSTANT:
					{
						res.r = border_const;
						res.g = border_const;
						res.b = border_const;
						res.a = border_const;
						break;
					}
				case qlm::BORDER::BORDER_REPLICATE:
					{
						x_idx = std::clamp(x_idx, 0, static_cast<int>(img_width) - 1);
						y_idx = std::clamp(y_idx, 0, static_cast<int>(img_height) - 1);
						res = in.getPixel(x_idx, y_idx);
						break;
					}
				case qlm::BORDER::BORDER_REFLECT:
					{
						x_idx = ReflectBorderIndex(x_idx, img_width);
						y_idx = ReflectBorderIndex(y_idx, img_height);
						res = in.getPixel(x_idx, y_idx);
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
			// Reset the sum array
			sum[0] = sum[1] = sum[2] = sum[3] = 0;
			
			for (int i = -pad_len_y; i <= pad_len_y; i++)
			{
				// get the pixel
				GetPixel(x, y + i); // Pixel in "res"

				MAC(sum, res, kernel_y.Get(i + pad_len_y));
			}
			// store the output
			//StorePix(sum, res);
			//out.setPixel(x, y, res);
		}
		// x-kernel
		for (int x = 0; x < img_width; x++)
		{
			// Reset
			sum[0] = sum[1] = sum[2] = sum[3] = 0;
			
			for (int i = -pad_len_x; i <= pad_len_x; i++)
			{
				// get the pixel
				GetPixel(x + i, y); // Pixel in "res"
				// use temp array
				MAC(sum, res, kernel_x.Get(i + pad_len_x));
			}
			// store the output
			StorePix(sum, res);
			out.setPixel(x, y, res);
		}
	}
}

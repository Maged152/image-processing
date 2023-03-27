#include "Filter2D.h"
#include <algorithm>

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

void qlm::Filter2D(const sf::Image& in, sf::Image& out, const qlm::Kernel& kernel, qlm::BORDER border, int border_const)
{
	static_assert(sizeof(in) == sizeof(out), "Input and output images must have the same size!");

	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;

	int ker_width = kernel.width / 2;
	int ker_height = kernel.height / 2;

	sf::Color res;
	float sum[4] = { 0 }; // r,g,b,a

	// top Border loop
	for (int y = 0; y < ker_height; y++)
	{
		for (int x = 0; x < img_width; x++)
		{
			// Reset the sum array for each pixel
			sum[0] = sum[1] = sum[2] = sum[3] = 0;
			for (int j = -ker_height; j <= ker_height; j++)
			{
				for (int i = -ker_width; i <= ker_width; i++)
				{
					// get the pixel
					int x_idx = x + i;
					int y_idx = y + j;
					if (x_idx >= 0 && x_idx < img_width && y_idx >= 0 && y_idx < img_height)
					{
						res = in.getPixel(x + i, y + j);
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
					
					MAC(sum, res, kernel.Get(i + ker_width, j + ker_height));
				}
			}
			// store the output
			StorePix(sum, res);
			out.setPixel(x, y, res);
		}
	}
	// bottom Border loop
	for (int y = img_height - ker_height; y < img_height; y++)
	{
		for (int x = 0; x < img_width; x++)
		{
			// Reset the sum array for each pixel
			sum[0] = sum[1] = sum[2] = sum[3] = 0;
			for (int j = -ker_height; j <= ker_height; j++)
			{
				for (int i = -ker_width; i <= ker_width; i++)
				{
					int x_idx = x + i;
					int y_idx = y + j;
					if (x_idx >= 0 && x_idx < img_width && y_idx >= 0 && y_idx < img_height)
					{
						res = in.getPixel(x + i, y + j);
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

					MAC(sum, res, kernel.Get(i + ker_width, j + ker_height));
				}
			}
			// store the output
			StorePix(sum, res);
			out.setPixel(x, y, res);
		}
	}
	// inner loop
	for (int y = ker_height; y < img_height - ker_height; y++)
	{
		for (int x = ker_width; x < img_width - ker_width; x++)
		{
			// Reset the sum array for each pixel
			sum[0] = sum[1] = sum[2] = sum[3] = 0;

			for (int j = -ker_height; j <= ker_height; j++)
			{
				for (int i = -ker_width; i <= ker_width; i++)
				{
					// get the pixel
					res = in.getPixel(x + i, y + j);

					MAC(sum, res, kernel.Get(i + ker_width, j + ker_height));
				}
			}
			// store the output
			StorePix(sum, res);
			out.setPixel(x, y, res);
		}
	}
}

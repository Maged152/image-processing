#include "Filter2D.h"



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
	unsigned int img_width = in.getSize().x;
	unsigned int img_height = in.getSize().y;

	int ker_width = kernel.width / 2;
	int ker_height = kernel.height / 2;
	// Border loop
	// TODO : handle borders
	// inner loop
	for (int y = ker_height; y < img_height - ker_height; y++)
	{
		for (int x = ker_width; x < img_width - ker_width; x++)
		{
			float sum[4] = {0}; // r,g,b,a
			for (int j = -ker_height; j <= ker_height; j++)
			{
				for (int i = -ker_width; i <= ker_width; i++)
				{
					// get the pixel
					sf::Color pix = in.getPixel(x + i, y + j);

					MAC(sum, pix, kernel.Get(i + ker_width, j + ker_height));
				}
			}
			// store the output
			sf::Color res;
			StorePix(sum, res);
			out.setPixel(x, y, res);
		}
	}
}

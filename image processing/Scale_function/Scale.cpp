#include "Scale.h"
#include <iostream>
#include <cmath>
#include <algorithm>
sf::Image& Scale(sf::Image& in, sf::Image& out, ScaleMethod method, float scale_x, float scale_y)
{
	unsigned int width = in.getSize().x;
	unsigned int height = in.getSize().y;
	// creat image with the new size
	unsigned int new_width = width * scale_x;
	unsigned int new_height = height * scale_y;

	out.create(new_width, new_height, sf::Color::Black);
	// check the scaling method
	if (method == ScaleMethod::NEAREST_NEIGHBOR)
	{
		for (int x = 0; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				// set the pixel with the nearest neighbor
				unsigned int nearest_x = std::clamp((unsigned int)std::round(x / scale_x), 0u, width - 1);
				unsigned int nearest_y = std::clamp((unsigned int)std::round(y / scale_y), 0u, height - 1);
				sf::Color c = in.getPixel(nearest_x, nearest_y);
				out.setPixel(x, y, c);
			}
		}
	}
	else if (method == ScaleMethod::BILINEAR)
	{
		for (int x = 0; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				// get the pixel with the nearest neighbor
				float nearest_x = x / scale_x;
				float nearest_y = y / scale_y;
				// get the four points
				unsigned int x1 = std::min((unsigned int)std::floor(nearest_x), width - 1);
				unsigned int y1 = std::min((unsigned int)std::floor(nearest_y), height - 1);
				unsigned int x2 = std::min((unsigned int)std::ceil(nearest_x), width - 1);
				unsigned int y2 = std::min((unsigned int)std::ceil(nearest_y), height - 1);
				// get the four neighbors
				sf::Color Q11 = in.getPixel(x1, y1);
				sf::Color Q12 = in.getPixel(x1, y2);
				sf::Color Q21 = in.getPixel(x2, y1);
				sf::Color Q22 = in.getPixel(x2, y2);
				// get the weights
				float normalize_factor = (x2 - x1) * (y2 - y1);
				float W11 = ((x2 - nearest_x)*(y2 - nearest_y)) / normalize_factor;
				float W12 = ((x2 - nearest_x) * (nearest_y - y1)) / normalize_factor;
				float W21 = ((nearest_x - x1) * (y2 - nearest_y)) / normalize_factor;
				float W22 = ((nearest_x - x1) * (nearest_y - y1)) / normalize_factor;
				// weighted sum
				sf::Color c = Q11 * W11 + Q12 * W12 + Q21 * W21 + Q22 * Q22;
				out.setPixel(x, y, c);
			}
		}
	}
	else
	{

	}

	return out;
}
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

	}
	else
	{

	}

	return out;
}
#include "Scale.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "color_operations.h"

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
			unsigned int x1 = std::floor(nearest_x);
			unsigned int y1 = std::floor(nearest_y);
			unsigned int x2 = std::min(x1 + 1, width - 1);
			unsigned int y2 = std::min(y1 + 1, height - 1);
			// get the four neighbors
			sf::Color top_left = in.getPixel(x1, y1);
			sf::Color top_right = in.getPixel(x2, y1);
			sf::Color bottom_left = in.getPixel(x1, y2);
			sf::Color bottom_right = in.getPixel(x2, y2);
			// offsets
			float x_offset = nearest_x - x1;
			float y_offset = nearest_y - y1;
			// interporate along x-axis
			sf::Color top = top_left * (1.0f - x_offset) + top_right * x_offset;
			sf::Color bottom = bottom_left * (1.0f - x_offset) + bottom_right * x_offset;
			// interporate along y-axis
			sf::Color out_pixel = top * (1.0f - y_offset) + bottom * y_offset;
			out.setPixel(x, y, out_pixel);
			 }
		}
	}
	else
	{

	}

	return out;
}
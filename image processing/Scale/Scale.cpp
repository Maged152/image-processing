#include "Scale.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include "color_operations.h"

void qlm::Scale(const sf::Image& in, sf::Image& out, qlm::ScaleMethod method, float scale_x, float scale_y)
{
	unsigned int width = in.getSize().x;
	unsigned int height = in.getSize().y;
	// creat image with the new size
	unsigned int new_width = width * scale_x;
	unsigned int new_height = height * scale_y;

	out.create(new_width, new_height, sf::Color::Black);
	// check the scaling method
	if (method == qlm::ScaleMethod::NEAREST_NEIGHBOR)
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
	else if (method == qlm::ScaleMethod::BILINEAR)
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
		for (int x = 0; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				// get the pixel with the nearest neighbor
				float nearest_x = x / scale_x;
				float nearest_y = y / scale_y;
				//
				unsigned int x1 = std::floor(nearest_x);
				unsigned int y1 = std::floor(nearest_y);
				// offsets
				float xoff = nearest_x - x1;
				float yoff = nearest_y - y1;
				float xxoff{ xoff * xoff }, xxxoff{ xoff * xoff * xoff };
				float yyoff{ yoff * yoff }, yyyoff{ yoff * yoff * yoff };

				struct FloatPixel 
				{
					float r, g, b, a;
				};
				// 16 point to sample
				FloatPixel bicubic_points[4][4]{};
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int px = std::clamp(x1 - 1 + i, 0u, width - 1);
						int py = std::clamp(y1 - 1 + j, 0u, height - 1);
						sf::Color p = in.getPixel(px, py);
						bicubic_points[i][j].r = (float)p.r;
						bicubic_points[i][j].g = (float)p.g;
						bicubic_points[i][j].b = (float)p.b;
						bicubic_points[i][j].a = (float)p.a;
					}
				}
				// x offsets
				float Q1_x = 0.5f * (-xxxoff + 2.0f * xxoff - xoff);
				float Q2_x = 0.5f * (3.0f * xxxoff - 5.0f * xxoff + 2.0f);
				float Q3_x = 0.5f * (-3.0f * xxxoff + 4.0f * xxoff + xoff);
				float Q4_x = 0.5f * (xxxoff - xxoff);
				// y offsets
				float Q1_y = 0.5f * (-yyyoff + 2.0f * yyoff - yoff);
				float Q2_y = 0.5f * (3.0f * yyyoff - 5.0f * yyoff + 2.0f);
				float Q3_y = 0.5f * (-3.0f * yyyoff + 4.0f * yyoff + yoff);
				float Q4_y = 0.5f * (yyyoff - yyoff);
				FloatPixel bicubic_points_y[4]{};
				for (int i = 0; i < 4; i++)
				{
					bicubic_points_y[i].r = bicubic_points[i][0].r * Q1_x + bicubic_points[i][1].r * Q2_x + bicubic_points[i][2].r * Q3_x + bicubic_points[i][3].r * Q4_x;
					bicubic_points_y[i].g = bicubic_points[i][0].g * Q1_x + bicubic_points[i][1].g * Q2_x + bicubic_points[i][2].g * Q3_x + bicubic_points[i][3].g * Q4_x;
					bicubic_points_y[i].b = bicubic_points[i][0].b * Q1_x + bicubic_points[i][1].b * Q2_x + bicubic_points[i][2].b * Q3_x + bicubic_points[i][3].b * Q4_x;
					bicubic_points_y[i].a = bicubic_points[i][0].a * Q1_x + bicubic_points[i][1].a * Q2_x + bicubic_points[i][2].a * Q3_x + bicubic_points[i][3].a * Q4_x;
				}
				sf::Color out_pixel;
				out_pixel.r =(uint8_t)std::clamp(bicubic_points_y[0].r * Q1_y + bicubic_points_y[1].r * Q2_y + bicubic_points_y[2].r * Q3_y + bicubic_points_y[3].r * Q4_y, 0.0f , 255.0f);
				out_pixel.g =(uint8_t)std::clamp(bicubic_points_y[0].g * Q1_y + bicubic_points_y[1].g * Q2_y + bicubic_points_y[2].g * Q3_y + bicubic_points_y[3].g * Q4_y, 0.0f , 255.0f);
				out_pixel.b =(uint8_t)std::clamp(bicubic_points_y[0].b * Q1_y + bicubic_points_y[1].b * Q2_y + bicubic_points_y[2].b * Q3_y + bicubic_points_y[3].b * Q4_y, 0.0f , 255.0f);
				out_pixel.a =(uint8_t)std::clamp(bicubic_points_y[0].a * Q1_y + bicubic_points_y[1].a * Q2_y + bicubic_points_y[2].a * Q3_y + bicubic_points_y[3].a * Q4_y, 0.0f , 255.0f);

				out.setPixel(x, y, out_pixel);
			}
		}

	}
}
#include "Scale/Scale.h"
#include <iostream>
#include <cmath>
#include <algorithm>

template<qlm::ImageFormat frmt, qlm::pixel_t T>
qlm::Image<frmt, T> qlm::Scale(const qlm::Image<frmt, T>& in, qlm::ScaleMethod method, float scale_x, float scale_y)
{
	unsigned int width = in.Width();
	unsigned int height = in.Height();
	// creat image with the new size
	unsigned int new_width = width * scale_x;
	unsigned int new_height = height * scale_y;

	qlm::Image<frmt, T> out;
	out.create(new_width, new_height, qlm::Pixel<frmt, T>{});
	// check the scaling method
	if (method == qlm::ScaleMethod::NEAREST_NEIGHBOR)
	{
		qlm::Pixel<frmt, T> c;
		for (int x = 0; x < new_width; x++)
		{
			for (int y = 0; y < new_height; y++)
			{
				// set the pixel with the nearest neighbor
				unsigned int nearest_x = std::clamp((unsigned int)std::round(x / scale_x), 0u, width - 1);
				unsigned int nearest_y = std::clamp((unsigned int)std::round(y / scale_y), 0u, height - 1);
				c = in.GetPixel(nearest_x, nearest_y);
				out.SetPixel(x, y, c);
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
			qlm::Pixel<frmt, T> top_left = in.GetPixel(x1, y1);
			qlm::Pixel<frmt, T> top_right = in.GetPixel(x2, y1);
			qlm::Pixel<frmt, T> bottom_left = in.GetPixel(x1, y2);
			qlm::Pixel<frmt, T> bottom_right = in.GetPixel(x2, y2);
			// offsets
			float x_offset = nearest_x - x1;
			float y_offset = nearest_y - y1;
			// interporate along x-axis
			qlm::Pixel<frmt, T> top = top_left * (1.0f - x_offset) + top_right * x_offset;
			qlm::Pixel<frmt, T> bottom = bottom_left * (1.0f - x_offset) + bottom_right * x_offset;
			// interporate along y-axis
			qlm::Pixel<frmt, T> out_pixel = top * (1.0f - y_offset) + bottom * y_offset;
			out.SetPixel(x, y, out_pixel);
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

				// 16 point to sample
				qlm::Pixel<frmt,float> bicubic_points[4][4]{};
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int px = std::clamp(x1 - 1 + i, 0u, width - 1);
						int py = std::clamp(y1 - 1 + j, 0u, height - 1);
						bicubic_points[i][j] = in.GetPixel(px, py);
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

				qlm::Pixel<frmt, float> bicubic_points_y[4]{};
				for (int i = 0; i < 4; i++)
				{
					bicubic_points_y[i] = bicubic_points[i][0] * Q1_x + bicubic_points[i][1] * Q2_x + bicubic_points[i][2] * Q3_x + bicubic_points[i][3] * Q4_x;
				}

				qlm::Pixel<frmt, T> out_pixel;
				out_pixel = bicubic_points_y[0] * Q1_y + bicubic_points_y[1] * Q2_y + bicubic_points_y[2] * Q3_y + bicubic_points_y[3] * Q4_y;

				out.SetPixel(x, y, out_pixel);
			}
		}
	}
	return out;
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t> 
qlm::Scale<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
										   qlm::ScaleMethod,
										   float,
										   float);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::Scale<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	qlm::ScaleMethod,
	float,
	float);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Scale<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	qlm::ScaleMethod,
	float,
	float);
// Explicit instantiation for GRAY , int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::Scale<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	qlm::ScaleMethod,
	float,
	float);

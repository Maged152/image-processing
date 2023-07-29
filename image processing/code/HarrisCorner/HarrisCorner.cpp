#include "HarrisCorner/HarrisCorner.h"
#include "ColorConvert/ColorConvert.h"
#include "Gaussian/Gaussian.h"
#include "SepFilter2D/SepFilter2D.h"
#include "Sobel/Sobel.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<KeyPoint<int>> HarrisCorner(const Image<frmt, T>& in, const unsigned int block_size, const unsigned int filter_size, float k, float threshold, const Border border_type, const int border_value)
	{
		const unsigned int width = in.Width();
		const unsigned int height = in.Height();

		Image<ImageFormat::GRAY, uint8_t> gray {width, height};
		
		if constexpr (frmt != ImageFormat::GRAY)
		{
			// convert to gray
			gray = ColorConvert<frmt, uint8_t, ImageFormat::GRAY, uint8_t>(in);
		}
		else
		{
			gray = in;
		}
		// call gaussian kernerl 
		int constexpr sigma = 1;
		Image<ImageFormat::GRAY, uint8_t> gaussian = Gaussian(gray, filter_size, sigma, sigma, border_type, border_value);
		// call sobel x and y
		Image<ImageFormat::GRAY, int16_t> sobel_x = SobelX<uint8_t, int16_t>(gaussian, filter_size, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> sobel_y = SobelY<uint8_t, int16_t>(gaussian, filter_size, border_type, border_value);
		// compute Ixx, Iyy, Ixy
		Image<ImageFormat::GRAY, int16_t> Ixx{ width , height};
		Image<ImageFormat::GRAY, int16_t> Iyy{ width , height };
		Image<ImageFormat::GRAY, int16_t> Ixy{ width , height };

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				auto Ix = sobel_x.GetPixel(x, y);
				auto Iy = sobel_y.GetPixel(x, y);

				Ixx.SetPixel(x, y, Ix * Ix);
				Ixy.SetPixel(x, y, Iy * Ix);
				Iyy.SetPixel(x, y, Iy * Iy);
			}
		}
		// sum on block size
		Kernel1D box{ block_size };
		for (int i = 0; i < block_size; i++)
		{
			box.Set(i, 1.0f / block_size);
		}
		Image<ImageFormat::GRAY, int16_t> Ixx_sum = SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(Ixx, box, box, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> Iyy_sum = SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(Iyy, box, box, border_type, border_value);
		Image<ImageFormat::GRAY, int16_t> Ixy_sum = SepFilter2D<ImageFormat::GRAY, int16_t, int16_t>(Ixy, box, box, border_type, border_value);

		Image<ImageFormat::GRAY, float> corners_response{ width, height };
		// compute R
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				float Ixx = Ixx_sum.GetPixel(x, y).v;
				float Iyy = Iyy_sum.GetPixel(x, y).v;
				float Ixy = Ixy_sum.GetPixel(x, y).v;

				float det = Ixx * Iyy - Ixy * Ixy;
				float tr = Ixx + Iyy;

				float response = det - k * tr * tr;
				// check if it is a corner
				if (response > threshold)
				{
					corners_response.SetPixel(x, y, response);
				}
				else
				{
					corners_response.SetPixel(x, y, std::numeric_limits<float>::min());
				}	
			}
		}
		std::vector<KeyPoint<int>> key_points;
		// non-max suppression
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				// check if it is a corner
				if (corners_response.GetPixel(x, y).v != std::numeric_limits<float>::min())
				{
					// 8-way non-max suppression
					float cur_response = corners_response.GetPixel(x, y).v;
					// offset arrays
					const int x_offset[3] = { x - 1 , x, x + 1 };
					const int y_offset[3] = { y - 1 , y, y + 1 };
					
					// top left neighbour
					if (x_offset[0] > 0 && y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// top neighbour
					if (y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[1], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// top right neighbour
					if (x_offset[2] < width && y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// left neighbour
					if (x_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[1]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// right neighbour
					if (x_offset[2] < width)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[1]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom left neighbour
					if (x_offset[0] > 0 && y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom neighbour
					if (y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[1], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom right neighbour
					if (x_offset[2] < width && y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// supress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					key_points.emplace_back(KeyPoint<int>{ { x, y}, cur_response});
				}		
			}
		}
		
		return key_points;
	}

	template std::vector<KeyPoint<int>>
	HarrisCorner<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, const unsigned int, const unsigned int, float, float, const Border, const int);
	
	template std::vector<KeyPoint<int>>
	HarrisCorner<ImageFormat::RGB, uint8_t>(const Image<ImageFormat::RGB, uint8_t>&, const unsigned int, const unsigned int, float, float, const Border, const int);

}

#include "HarrisCorner.hpp"
#include "Gaussian.hpp"
#include "common/StructureTensor.hpp"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<KeyPoint<int>> HarrisCorner(const Image<frmt, T>& in, const int block_size, const int filter_size, float k, float threshold, const  BorderMode<frmt, T>& border_mode)
	{
		const int width = in.Width();
		const int height = in.Height();

		// normalization factor
		float scale = (1 << (filter_size - 1)) * block_size * std::numeric_limits<T>::max();
		scale = 1.0 / scale;
		float normalization = scale * scale * scale * scale;

		// call Gaussian kernel 
		constexpr int sigma = 1;
		Image<ImageFormat::GRAY, uint8_t> gaussian = Gaussian(in, filter_size, sigma, sigma, border_mode);

		// compute I_x, I_y and the box-filtered structure tensor sums
		Image<ImageFormat::GRAY, int16_t> I_x, I_y;
		Image<ImageFormat::GRAY, float> S_xx, S_yy, S_xy;

		BorderMode<ImageFormat::GRAY, float> border_mode_f{};
		border_mode_f.border_type = border_mode.border_type;
		border_mode_f.border_pixel = border_mode.border_pixel.v;

		StructureTensor(gaussian, filter_size, block_size, block_size, I_x, I_y, S_xx, S_yy, S_xy, border_mode, border_mode_f);

		// compute R
		Image<ImageFormat::GRAY, float> corners_response = CornerResponse(S_xx, S_yy, S_xy, k, normalization);
		
		// check if it is a corner
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (corners_response.GetPixel(x, y).v <= threshold)
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
					
					// top left neighbor
					if (x_offset[0] > 0 && y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// top neighbor
					if (y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[1], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// top right neighbor
					if (x_offset[2] < width && y_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[0]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// left neighbor
					if (x_offset[0] > 0)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[1]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// right neighbor
					if (x_offset[2] < width)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[1]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom left neighbor
					if (x_offset[0] > 0 && y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[0], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom neighbor
					if (y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[1], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
							corners_response.SetPixel(x, y, std::numeric_limits<float>::min());

							continue;
						}
					}

					// bottom right neighbor
					if (x_offset[2] < width && y_offset[2] < height)
					{
						float neighbour_response = corners_response.GetPixel(x_offset[2], y_offset[2]).v;
						if (cur_response <= neighbour_response)
						{
							// suppress this response
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
	HarrisCorner<ImageFormat::GRAY, uint8_t>(const Image<ImageFormat::GRAY, uint8_t>&, const int, const int, float, float, const BorderMode<ImageFormat::GRAY, uint8_t>&);
	
}

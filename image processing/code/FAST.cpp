#include "FAST.h"
#include <cmath>

namespace qlm
{
	template<pixel_t T>
	std::vector<KeyPoint<int>> FAST(const Image<ImageFormat::GRAY, T>& in, 
								    const unsigned int arc_length,
		                            const T threshold,
		                            const bool non_max_suppression)
	{
		std::vector<KeyPoint<int>> key_points;
		// temp buffer to hold the potential key-points
		T* buff = new T[in.Width() * in.Height()];
		std::memset(buff, 1, in.Width() * in.Height() * sizeof(T));

		// offsets of the 16-point (circle)
		constexpr int x_off[16] = { 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1 };
		constexpr int y_off[16] = { -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3 };
		
		// offsets for non-max suppression
		constexpr int x_nonmax_off[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		constexpr int y_nonmax_off[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };

		// check for n consecutive bits
		auto has_n_consecutive_set_bits = [](const auto num, const int n, const int num_bits)
		{
			int mask = (1 << n) - 1;
			long shifted_buffer = num | (num << num_bits);

			for (int i = 0; i < num_bits; i++)
			{
				if ((shifted_buffer & mask) == mask)
				{
					return true;
				}

				shifted_buffer >>= 1;
			}

			return false;
		};

		// create brighter/darker checker
		auto dark_bright_check = [&](int len, int x, int y, uint32_t& brighter, uint32_t& darker, const int* offx, const int* offy)
		{
			auto curr_pixel = in.GetPixel(x, y);

			int lower_threshold = curr_pixel.v - threshold;
			int upper_threshold = curr_pixel.v + threshold;

			for (int i = 0; i < len; i++)
			{
				int pix = in.GetPixel(x + offx[i], y + offy[i]).v;

				brighter = brighter << 1;
				darker = darker << 1;

				if (pix > upper_threshold)
				{
					brighter |= 1;
				}
				else if (pix < lower_threshold)
				{
					darker |= 1;
				}
			}
		};
		
		// quick check
		auto quick_check = [&](const int n_consecutive)
		{
			constexpr int x_qc_off[16] = { 0, 3, 0, -3};
			constexpr int y_qc_off[16] = { -3, 0, 3, 0};

			for (int y = 3; y < in.Height() - 3; y++)
			{
				for (int x = 3; x < in.Width() - 3; x++)
				{
					uint32_t darker{ 0 }, brighter{ 0 };

					// check the 4 point
					dark_bright_check(4, x, y, brighter, darker, x_qc_off, y_qc_off);

					// check for n_consecutive consecutive pixels
					bool is_corner = has_n_consecutive_set_bits(brighter, n_consecutive, 4);

					if (!is_corner)
					{
						// check for darker 
						is_corner = has_n_consecutive_set_bits(darker, n_consecutive, 4);

						if (!is_corner)
						{
							// not a corner
							buff[y * in.Width() + x] = 0;
						}
					}

				}
			}
		};

		// calculate response
		auto fast_response = [&, x_off, y_off](const int x, const int y)
		{
			int curr_pixel = in.GetPixel(x, y).v;

			int response{0};
			
			// loop over the 16-neighbor pixels
			for (int p = 0; p < 16; p++)
			{
				int p_th { 255 };

				// loop over the arc length
				for (int i = 0; i < arc_length; i++)
				{
					int x_idx = x_off[(i + p) % 16] + x;
					int y_idx = y_off[(i + p) % 16] + y;

					int pix = in.GetPixel(x_idx, y_idx).v;

					p_th = std::min(p_th, std::abs(pix - curr_pixel));
					
				}

				response = std::max(response, p_th);
			}
			
			return response;
		};

		if (arc_length == 12)
		{
			// 3 consecutive pixels must satisfy the condition
			quick_check(3);
		}
		else if (arc_length == 9)
		{
			// 2 consecutive pixels must satisfy the condition
			quick_check(2);
		}
		
		
		
		// check valid key-points
		for (int y = 3; y < in.Height() - 3; y++)
		{
			for (int x = 3; x < in.Width() - 3; x++)
			{
				if (buff[y * in.Width() + x])
				{
					uint32_t darker{0}, brighter{0};

					// check the 16 point
					dark_bright_check(16, x, y, brighter, darker, x_off, y_off);

					// check for n consecutive pixels
					bool is_corner = has_n_consecutive_set_bits(brighter, arc_length, 16);

					if (non_max_suppression)
					{
						if (is_corner)
						{
							// calculate the response
							buff[y * in.Width() + x] = fast_response(x, y);
						}
						else
						{
							// check for darker 
							is_corner = has_n_consecutive_set_bits(darker, arc_length, 16);

							buff[y * in.Width() + x] = is_corner ? fast_response(x, y) : 0;
						}
					}
					else
					{
						if (is_corner)
						{
							// store key-point
							Point<int> kp{ x, y };
							key_points.emplace_back(KeyPoint<int>{kp});	
						}
						else
						{
							// check for darker 
							is_corner = has_n_consecutive_set_bits(darker, arc_length, 16);

							if (is_corner)
							{
								// store key-point
								Point<int> kp{ x, y };
								key_points.emplace_back(KeyPoint<int>{kp});
							}
						}
					}

					
				}
			}
		}

		if (non_max_suppression)
		{
			for (int y = 4; y < in.Height() - 4; y++)
			{
				for (int x = 4; x < in.Width() - 4; x++)
				{
					T response = buff[y * in.Width() + x];

					if (response)
					{
						T max_res = buff[(y + y_nonmax_off[0]) * in.Width() + (x + x_nonmax_off[0])];
						for (int i = 1; i < 8; i++)
						{
							max_res = std::max(max_res, buff[(y + y_nonmax_off[i]) * in.Width() + (x + x_nonmax_off[i])]);
						}

						if (response > max_res)
						{
							// store key-point
							Point<int> kp{ x, y };
							key_points.emplace_back(KeyPoint<int>{kp, (float)response});

						}
					}
				}
			}
		}


		delete[] buff;

		return key_points;
	}

	template std::vector<KeyPoint<int>> FAST(const Image<ImageFormat::GRAY, uint8_t>&, const unsigned int, const uint8_t, const bool);

}
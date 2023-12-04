#include "FAST/FAST.h"

namespace qlm
{
	template<pixel_t T>
	std::vector<KeyPoint<int>> FAST(const Image<ImageFormat::GRAY, T>& in, 
								    const int arc_length,
		                            const int threshold, 
		                            const bool non_max_suppression)
	{
		std::vector<KeyPoint<int>> key_points;
		// temp buffer to hold the potential key-points
		T* buff = new T[in.Width() * in.Height()];
		std::memset(buff, 1, in.Width() * in.Height() * sizeof(T));

		// offsets of the 16-point
		int x_off[16] = { 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1 };
		int y_off[16] = { -3, -3, -2, -1, 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3 };
		
		// offsets for non-max suppression
		int x_nonmax_off[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int y_nonmax_off[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		// quick check
		// TODO
		// check for n consecutive bits
		auto has_n_consecutive_set_bits = [](auto num, int n, int num_bits)
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
		// check valid key-points
		for (int y = 0; y < in.Height(); y++)
		{
			for (int x = 0; x < in.Width(); x++)
			{
				if (buff[y * in.Width() + x])
				{
					auto curr_pixel = in.GetPixel(x, y);

					int lower_threshold = curr_pixel.v - threshold;
					int upper_threshold = curr_pixel.v + threshold;

					uint32_t darker{0}, brighter{0};
					// loop over the 16 point
					for (int i = 0; i < 16; i++)
					{
						int pix = in.GetPixel(x + x_off[i], y + y_off[i]).v;

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

					// check for n consecutive pixels
					bool is_corner = has_n_consecutive_set_bits(brighter, arc_length, 16);

					if (!is_corner)
					{
						buff[y * in.Width() + x] = 0;
					}
					else
					{
						// check for darker 
						is_corner = has_n_consecutive_set_bits(darker, arc_length, 16);

						if (!is_corner)
						{
							buff[y * in.Width() + x] = 0;
						}
						else if (!non_max_suppression)
						{
							// store key-point
							Point<int> kp{x, y};
							key_points.emplace_back(KeyPoint<int>{kp});
						}
					}
				}
			}
		}

		if (non_max_suppression)
		{
			for (int y = 0; y < in.Height(); y++)
			{
				for (int x = 0; x < in.Width(); x++)
				{
					if (buff[y * in.Width() + x])
					{
						// TODO
					}
				}
			}
		}

		return key_points;
	}

	template std::vector<KeyPoint<int>> FAST(const Image<ImageFormat::GRAY, uint8_t>&, const int, const int, const bool);

}
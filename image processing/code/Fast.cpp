#include "FAST/FAST.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<KeyPoint<int>> FAST(const Image<frmt, T>& in, int arc_length, int threshold, bool non_max_suppression)
	{
		// offsets of the 16-point
		int x_off[16] = { 0, 1, 2, 3, 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1 };
		int x_off[16] = { 3, 3, 2, 1, 0, -1, -2, -3, -3, -3, -2, -1, 0, 1, 2, 3 };
		
		// quick check

		// check valid key-points
		int16_t darker, brighter;

		return std::vector<KeyPoint<int>>();
	}
}
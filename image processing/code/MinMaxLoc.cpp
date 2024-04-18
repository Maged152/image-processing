#include "MinMaxLoc.h"

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc(const Image<frmt, T>& in)
	{
		T min_val = std::numeric_limits<T>::max();
		T max_val = std::numeric_limits<T>::lowest();

		const size_t len = in.width * in.height;

		// find min and max value
		for (int i = 0; i < len; i++)
		{
			if constexpr (frmt == ImageFormat::GRAY)
			{
				T in_val = in.GetPixel(i).v;
				if (in_val > max_val)
				{
					max_val = in_val;
				}

				if (in_val < min_val)
				{
					min_val = in_val;
				}
			}
		}

		std::pair<std::vector<Point<int>>, std::vector<Point<int>>> locations;

		for (int y = 0; y < in.height; y++)
		{
			for (int x = 0; x < in.width; x++)
			{
				if constexpr (frmt == ImageFormat::GRAY)
				{
					T in_val = in.GetPixel(x, y).v;
					if (in_val == min_val)
					{
						locations.first.emplace_back(Point<int> {x, y});
					}

					if (in_val == max_val)
					{
						locations.second.emplace_back(Point<int>{ x, y });
					}
				}
			}
		}

		return locations;
	}

	template std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc
	(const Image<ImageFormat::GRAY, float>&);

	template std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc
	(const Image<ImageFormat::GRAY, uint8_t>&);
}
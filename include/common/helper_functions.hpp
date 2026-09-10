#pragma once
#include <algorithm>
#include <tuple>
#include "types/containers.hpp"

namespace qlm
{
    // Variadic template function to find the min/max value among a list of arguments
    template <typename T>
    T Min(T value)
    {
        return value;
    }

    template <typename T, typename... Args>
    T Min(T first, Args... rest)
    {
        return std::min(first, Min(rest...));
    }

    template <typename T>
    T Max(T value)
    {
        return value;
    }

    template <typename T, typename... Args>
    T Max(T first, Args... rest)
    {
        return std::max(first, Max(rest...));
    }

    // Function to calculate the valid ROI (Region of Interest) within the image boundaries
    template<class T = int>
	std::tuple<T, T, T, T> ROI(const Rectangle<T>& roi, const int img_width, const int img_height)
	{
		const T start_x = std::max(roi.top_left.x, 0);
		const T start_y = std::max(roi.top_left.y, 0);

		const T valid_width = (roi.width <= 0 || roi.width > img_width - start_x) ? img_width - start_x : roi.width;
		const T valid_height = (roi.height <= 0 || roi.height > img_height - start_y) ? img_height - start_y : roi.height;

		const T end_x = start_x + valid_width;
		const T end_y = start_y + valid_height;

		return std::make_tuple(start_x, start_y, end_x, end_y);
	}
}
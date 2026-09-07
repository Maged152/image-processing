#pragma once

#include "types/TransformationMatrix.hpp"
#include "types/Kernel.hpp"
#include "types/Interpolation.hpp"
#include "types/matrix.hpp"
#include "types/enums.hpp"
#include "types/KDTree.hpp"
#include "types/containers.hpp"
#include <vector>
#include <limits>
#include <array>
#include <unordered_map>
#include <tuple>

namespace qlm
{
	struct TermCriteria
	{
		int max_count = 0;
		double epsilon = 0.0;
	};

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
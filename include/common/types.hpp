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

namespace qlm
{
	struct TermCriteria
	{
		int max_count = 0;
		double epsilon = 0.0;
	};
}
#include "WarpAffine/WarpAffine.h"
#include <cmath>
#include <numbers>

namespace qlm
{
	TransformationMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale)
	{
		TransformationMatrix out;

		// degree to radian
		const float angle_r = angle * std::numbers::pi / 180.0f;

		float alpha = scale * std::cos(angle_r);
		float beta = scale * std::sin(angle_r);

		out.SetElement(0, 0, alpha);
		out.SetElement(0, 1, beta);
		out.SetElement(1, 0, -beta);
		out.SetElement(1, 1, alpha);

		float m02 = (1 - alpha) * center.x - beta * center.y;
		float m12 = beta * center.x + (1 - alpha) * center.y;

		out.SetElement(0, 2, m02);
		out.SetElement(1, 2, m12);

		return out;
	}

	TransformationMatrix GetAffineTransform(const Point<int> src[3], const Point<int> dst[3])
	{
		TransformationMatrix out;


		return out;
	}
}
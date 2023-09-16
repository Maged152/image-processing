#include "WarpAffine/WarpAffine.h"
#include <cmath>

namespace qlm
{
	TransformationMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale)
	{
		TransformationMatrix out;

		float alpha = scale * std::cosf(angle);
		float beta = scale * std::sinf(angle);

		out.SetElement(0, 0, alpha);
		out.SetElement(0, 1, beta);
		out.SetElement(1, 0, -beta);
		out.SetElement(1, 1, alpha);

		float m02 = (1 - alpha) * center.x - beta * center.y;
		float m12 = beta * center.x + (1 - beta) * center.y;

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
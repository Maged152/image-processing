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

		// Gaussian elimination
		float aug_mat_x[3][4]{};
		float aug_mat_y[3][4]{};
		// construct the augmented matrix with the input points
		/*
			x_n = m00 * x + m01 * y + m02
			B = AX
			|x y 1 | x_n|
			| ......... |
			| ......... |

			the same for y
			y_n = m10 * x + m11 * y + m12
			|x y 1 | y_n|
			| ......... |
			| ......... |
		*/
		for (int i = 0; i < 3; i++)
		{
			aug_mat_x[i][0] = src[i].x;
			aug_mat_x[i][1] = src[i].y;
			aug_mat_x[i][2] = 1.0f;
			aug_mat_x[i][3] = dst[i].x;

			aug_mat_y[i][0] = src[i].x;
			aug_mat_y[i][1] = src[i].y;
			aug_mat_y[i][2] = 1.0f;
			aug_mat_y[i][3] = dst[i].y;
		}
		
		// function to swap two rows
		auto SwapRows = [](float** matrix, int row1, int row2)
		{
			float* temp = matrix[row1];
			matrix[row1] = matrix[row2];
			matrix[row2] = temp;
			temp = nullptr;
		};
		
		// function to find the pivot

		return out;
	}
}
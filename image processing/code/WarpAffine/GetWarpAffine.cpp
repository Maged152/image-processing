#include "WarpAffine.h"
#include "common/solve.h"
#include <cmath>
#include <numbers>

namespace qlm
{
	AffineMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale)
	{
		AffineMatrix out;

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

	AffineMatrix GetAffineTransform(const Point<int> src[3], const Point<int> dst[3])
	{
		AffineMatrix out;

		// Gaussian elimination
		float aug_mat_x[3][4]{};
		float sol_mat_x[3][1]{};

		float aug_mat_y[3][4]{};
		float sol_mat_y[3][1]{};
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

		qlm::Matrix augmented_matrix_x{ (float*)aug_mat_x, 3, 4 };
		qlm::Matrix solution_matrix_x{ (float*)sol_mat_x, 3, 1 };

		qlm::Matrix augmented_matrix_y{ (float*)aug_mat_x, 3, 4 };
		qlm::Matrix solution_matrix_y{ (float*)sol_mat_x, 3, 1 };

		Solve(augmented_matrix_x, solution_matrix_x);
		Solve(augmented_matrix_y, solution_matrix_y);

		out.SetElement(0, solution_matrix_x.Get(0));
		out.SetElement(1, solution_matrix_x.Get(1));
		out.SetElement(2, solution_matrix_x.Get(2));

		out.SetElement(3, solution_matrix_y.Get(0));
		out.SetElement(4, solution_matrix_y.Get(1));
		out.SetElement(5, solution_matrix_y.Get(2));

		augmented_matrix_x.SetToNull();
		solution_matrix_x.SetToNull();

		augmented_matrix_y.SetToNull();
		solution_matrix_y.SetToNull();


		return out;
	}
}
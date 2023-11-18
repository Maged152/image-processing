#include "WarpPerspective/WarpPerspective.h"
#include "common/solve.h"

namespace qlm
{
	PerspectiveMatrix GetPerspectiveTransform(const Point<int> src[4], const Point<int> dst[4])
	{
		PerspectiveMatrix out;

		// augmented matrices
		float aug_mat[8][9]{};
		float sol_mat[8][1]{};

		// construct the augmented matrix with the input points
		/*
			x_out = (m11 * x + m12 * y + m13) / (m31 * x + m32 * y + 1)
			y_out = (m21 * x + m22 * y + m23) / (m31 * x + m32 * y + 1)

			x_out = m11 * x + m12 * y + m13 - m31 * x * x_out - m32 * y * x_out
			y_out = m21 * x + m22 * y + m23 - m31 * x * y_out - m32 * y * y_out

			B = AX
			|x y 1 0 0 0 -x*x_out -y*x_out| x_out|
			| .................................. |
			|0 0 0 x y 1 -x*y_out -y*y_out| y_out|
			|................................... |
		*/
		for (int i = 0; i < 4; i++)
		{
			aug_mat[i][0] = aug_mat[i + 4][3] = src[i].x;
			aug_mat[i][1] = aug_mat[i + 4][4] = src[i].y;
			aug_mat[i][2] = aug_mat[i + 4][5] = 1.0f;

			aug_mat[i][3] = aug_mat[i + 4][0] = 0.0f;
			aug_mat[i][4] = aug_mat[i + 4][1] = 0.0f;
			aug_mat[i][5] = aug_mat[i + 4][2] = 0.0f;

			aug_mat[i][6] = -src[i].x * dst[i].x;
			aug_mat[i][7] = -src[i].y * dst[i].x;

			aug_mat[i + 4][6] = -src[i].x * dst[i].y;
			aug_mat[i + 4][7] = -src[i].y * dst[i].y;

			aug_mat[i][8] = dst[i].x;
			aug_mat[i + 4][8] = dst[i].y;
		}

		qlm::Matrix augmented_matrix{ (float*)aug_mat, 8, 9 };
		qlm::Matrix solution_matrix{ (float*)sol_mat, 8, 1 };

		Solve(augmented_matrix, solution_matrix);

		for (int i = 0; i < 8; i++)
		{
			out.SetElement(i, sol_mat[i][0]);
		}
		out.SetElement(8, 1);

		augmented_matrix.SetToNull();
		solution_matrix.SetToNull();

		return out;
	}
}
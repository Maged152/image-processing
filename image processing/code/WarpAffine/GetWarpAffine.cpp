#include "WarpAffine/WarpAffine.h"
#include <cmath>
#include <numbers>
#include <iostream>

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

	void print(float aug_mat_x[3][4])
	{
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 4; c++)
			{
				std::cout << aug_mat_x[r][c] << " ";
			}
			std::cout << '\n';
		}
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
		
		std::cout << "constructed x\n";
		print(aug_mat_x);

		std::cout << "constructed y\n";
		print(aug_mat_x);
		// function to swap two rows
		auto SwapRows = [](float matrix[3][4], int row1, int row2)
		{
			std::cout << "before swap " << row1 << "  and  " << row2 << '\n';
			print(matrix);
			for (int i = 0; i < 4; ++i) 
			{
				float temp = matrix[row1][i];
				matrix[row1][i] = matrix[row2][i];
				matrix[row2][i] = temp;
			}
			std::cout << "after swap " << row1 << "  and  " << row2 << '\n';
			print(matrix);
		};
		
		// function to find the pivot
		auto Pivot = [](float matrix[3][4], int row)
		{
			int pivot_row = row;

			for (int r = row + 1; r < 3; r++)
			{
				if (std::abs(matrix[r][row]) > std::abs(matrix[row][row]))
				{
					pivot_row = r;
				}
			}
			return pivot_row;
		};

		// do Gaussian elimination
		auto DoElimination = [&SwapRows, &Pivot](float matrix[3][4])
		{
			
			for (int r = 0; r < 2; r++)
			{
				std::cout << "start elimination "  << r << '\n';
				print(matrix);
				// find the pivot in location [r,r]
				int pivot = Pivot(matrix, r);

				if (pivot != r)
				{
					SwapRows(matrix, r, pivot);
				}

				// do elimination
				float piv = matrix[r][r];
				for (int e = r; e < 2; ++e)
				{
					
					float lead = matrix[e + 1][e];
					for (int c = r; c < 4; ++c)
					{
						matrix[e + 1][c] = (matrix[r][c] / piv) * lead - matrix[e + 1][c];
					}
				}

				std::cout << "end elimination " << r << '\n';
				print(matrix);
			}
			std::cout << "after elimination " << '\n';
			print(matrix);
		};
		
		// back substitution
		auto Backsubstitution = [&out](float matrix[3][4], int index)
		{
			std::cout << "before Backsubstitution " << '\n';
			print(matrix);
			for (int r = 2; r >= 0; r--)
			{
				float res = matrix[r][3];
				// TODO check this condition and set/get 
				for (int c = r; c < 2; c++)
				{
					res -= matrix[r][c + 1] * out.GetElement(index, c + 1);
				}
				out.SetElement(index, r, res / matrix[r][r]);
			}

			std::cout << "after Backsubstitution " << '\n';
			print(matrix);

		};
		
		DoElimination(aug_mat_x);
		DoElimination(aug_mat_y);

		Backsubstitution(aug_mat_x, 0);
		Backsubstitution(aug_mat_y, 1);


		return out;
	}
}
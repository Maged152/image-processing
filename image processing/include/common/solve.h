#pragma once

#include <cmath>
#include <numbers>
#include "types.h"


namespace qlm
{
	// Gaussian elimination 
	inline void Solve(Matrix& aug_mat, Matrix& sol)
	{
		// check the dimensions
		if ((aug_mat.Rows() + 1) != aug_mat.Columns() &&
			sol.Rows() != sol.Columns() &&
			sol.Rows() != aug_mat.Rows() &&
			sol.Columns() != 1)
		{
			return;
		}

		// function to swap two rows
		auto SwapRows = [](Matrix& aug_mat, int row1, int row2)
		{
			for (int i = 0; i < aug_mat.Columns(); ++i)
			{
				float temp = aug_mat.Get(row1,i);
				aug_mat.Set(row1, i, aug_mat.Get(row2, i));
				aug_mat.Set(row2, i, temp);
			}
		};

		// function to find the pivot (largest)
		auto Pivot = [](const Matrix& aug_mat, int start_row)
		{
			int pivot_row = start_row;
			int max_val = aug_mat.Get(start_row, start_row);

			for (int r = start_row + 1; r < aug_mat.Rows(); r++)
			{
				if (std::abs(aug_mat.Get(r, start_row)) > std::abs(max_val))
				{
					pivot_row = r;
					max_val = aug_mat.Get(r, start_row);
				}
			}
			return pivot_row;
		};

		// do Gaussian elimination
		auto DoElimination = [&SwapRows, &Pivot](Matrix& aug_mat)
		{

			for (int r = 0; r < aug_mat.Rows() - 1; r++)
			{
				// find the pivot in location [r,r]
				int pivot = Pivot(aug_mat, r);

				if (pivot != r)
				{
					SwapRows(aug_mat, r, pivot);
				}

				// do elimination
				float piv = aug_mat.Get(r, r);
				for (int cur_r = r; cur_r < aug_mat.Rows() - 1; ++cur_r)
				{

					float lead = aug_mat.Get(cur_r + 1, r);
					for (int c = r; c < aug_mat.Columns(); ++c)
					{
						float val = (aug_mat.Get(r, c) / piv) * lead - aug_mat.Get(cur_r + 1, c);
						aug_mat.Set(cur_r + 1, c, val);
					}
				}
			}
		};

		// back substitution
		auto Backsubstitution = [](const Matrix& aug_mat, Matrix& sol)
		{
			for (int r = aug_mat.Rows() - 1; r >= 0; r--)
			{
				float res = aug_mat.Get(r, aug_mat.Columns() - 1);
				
				for (int c = aug_mat.Columns() - 2; c != r; c--)
				{
					res -= aug_mat.Get(r, c) * sol.Get(c, 0);
				}
				sol.Set(r, 0, res / aug_mat.Get(r, r));
			}
		};

		DoElimination(aug_mat);
		Backsubstitution(aug_mat, sol);
	}
}
#include "WarpAffine.hpp"
#include <cmath>
#include <utility>
#include <algorithm>

namespace qlm
{
	std::pair<bool, AffineMatrix> InverseAffineMatrix(const AffineMatrix& mat)
	{
		AffineMatrix mat_inv = AffineMatrix{};

		float det = mat.GetElement(0, 0) * mat.GetElement(1, 1) - mat.GetElement(0, 1) * mat.GetElement(1, 0);
		// check determinant (guards against singular and near-singular matrices)
		if (std::abs(det) < 1e-6f)
		{
			return std::make_pair(false, AffineMatrix{});
		}
		mat_inv.SetElement(0, 0, mat.GetElement(1, 1) / det);
		mat_inv.SetElement(1, 1, mat.GetElement(0, 0) / det);

		mat_inv.SetElement(0, 1, -1 * mat.GetElement(0, 1) / det);
		mat_inv.SetElement(1, 0, -1 * mat.GetElement(1, 0) / det);
		
		float m02 = ((mat.GetElement(0, 1) * mat.GetElement(1, 2)) - (mat.GetElement(1, 1) * mat.GetElement(0, 2))) / det;
		float m12 = ((mat.GetElement(1, 0) * mat.GetElement(0, 2)) - (mat.GetElement(0, 0) * mat.GetElement(1, 2))) / det;
		
		mat_inv.SetElement(0, 2, m02);
		mat_inv.SetElement(1, 2, m12);

		return std::make_pair(true, mat_inv);
	}

	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(const Image<frmt, T>& in, const AffineMatrix& mat, const int dst_width, const int dst_height, const InterpolationFlag inter, const BorderMode<frmt, T>& border_mode)
	{
		// create the output image
		Image<frmt, T> out;
		out.Create(dst_width, dst_height,border_mode.border_pixel);
	
		auto [is_invertible, mat_inv] = InverseAffineMatrix(mat);
		if (!is_invertible)
		{
			return out;
		}

		// do transformation
		auto transform_out_in = [&](int out_x, int out_y)
		{
			float in_x = out_x * mat_inv.GetElement(0, 0) + out_y * mat_inv.GetElement(0, 1) + mat_inv.GetElement(0, 2);
			float in_y = out_x * mat_inv.GetElement(1, 0) + out_y * mat_inv.GetElement(1, 1) + mat_inv.GetElement(1, 2);

			return std::pair<float, float>(in_x, in_y);
		};

		auto transform_in_out = [&](int in_x, int in_y)
		{
			float out_x = in_x * mat.GetElement(0, 0) + in_y * mat.GetElement(0, 1) + mat.GetElement(0, 2);
			float out_y = in_x * mat.GetElement(1, 0) + in_y * mat.GetElement(1, 1) + mat.GetElement(1, 2);

			return std::pair<float, float>(out_x, out_y);
		};
		
		// Start/end index
		const auto [tl_x, tl_y] = transform_in_out(0, 0);
		const auto [tr_x, tr_y] = transform_in_out(in.width - 1, 0);
		const auto [bl_x, bl_y] = transform_in_out(0, in.height - 1);
		const auto [br_x, br_y] = transform_in_out(in.width - 1, in.height -1);
		
		const int start_x = std::floor(qlm::Min(tl_x, tr_x, br_x, bl_x, 0.0f));
		const int end_x   = std::ceil(qlm::Max(tl_x, tr_x, br_x, bl_x, static_cast<float>(dst_width - 1)));

		const int start_y = std::floor(qlm::Min(tl_y, tr_y, br_y, bl_y, 0.0f));
		const int end_y   = std::ceil(qlm::Max(tl_y, tr_y, br_y, bl_y, static_cast<float>(dst_height - 1)));
		
		// loop over the output image
		for (int y = start_y; y <= end_y; y++)
		{
			for (int x = start_x; x <= end_x; x++)
			{
				const auto [in_x, in_y] = transform_out_in(x, y);

				// check if inside the image
				if (in_x < 0 || in_y < 0 || in_x >= in.width || in_y >= in.height)
					continue;
				
				const auto pix = Interpolation(in, in_x, in_y, inter, border_mode);
				out.SetPixel(x, y, pix);
			}
		}
		return out;
	}

	template Image<ImageFormat::RGB, uint8_t> WarpAffine(const Image<ImageFormat::RGB, uint8_t >&,
				const AffineMatrix&, const int, const int,
				const InterpolationFlag, const BorderMode<ImageFormat::RGB, uint8_t>&);


	template Image<ImageFormat::GRAY, uint8_t> WarpAffine(const Image<ImageFormat::GRAY, uint8_t >&,
		const AffineMatrix&, const int, const int,
		const InterpolationFlag, const BorderMode<ImageFormat::GRAY, uint8_t>&);

}
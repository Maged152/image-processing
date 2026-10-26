#include "common/StructureTensor.hpp"
#include "Sobel.hpp"
#include "Multiply.hpp"
#include "BoxFilter.hpp"
#include <cassert>

namespace qlm
{
	void StructureTensor(
		const Image<ImageFormat::GRAY, uint8_t>& in,
		const int gradient_size,
		const int block_x_size,
		const int block_y_size,
		      Image<ImageFormat::GRAY, int16_t>& I_x,
		      Image<ImageFormat::GRAY, int16_t>& I_y,
		      Image<ImageFormat::GRAY, float>& S_xx,
		      Image<ImageFormat::GRAY, float>& S_yy,
		      Image<ImageFormat::GRAY, float>& S_xy,
		const BorderMode<ImageFormat::GRAY, uint8_t>& border_mode,
		const BorderMode<ImageFormat::GRAY, float>& box_border_mode,
		const Rectangle<int>& roi)
	{
		// sobel derivatives (full-size images; only the roi region is filled)
		I_x = SobelX<uint8_t, int16_t>(in, gradient_size, border_mode, roi);
		I_y = SobelY<uint8_t, int16_t>(in, gradient_size, border_mode, roi);

		// compute I_xx, I_yy, I_xy as temporaries
		const Image<ImageFormat::GRAY, float> I_xx = Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_x, 1.0f, OverFlowFlag::WRAP, roi);
		const Image<ImageFormat::GRAY, float> I_yy = Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_y, I_y, 1.0f, OverFlowFlag::WRAP, roi);
		const Image<ImageFormat::GRAY, float> I_xy = Multiply<ImageFormat::GRAY, int16_t, int16_t, float>(I_x, I_y, 1.0f, OverFlowFlag::WRAP, roi);

		// sum over the block window
		S_xx = BoxFilter<ImageFormat::GRAY, float, float>(I_xx, block_x_size, block_y_size, false, box_border_mode, roi);
		S_yy = BoxFilter<ImageFormat::GRAY, float, float>(I_yy, block_x_size, block_y_size, false, box_border_mode, roi);
		S_xy = BoxFilter<ImageFormat::GRAY, float, float>(I_xy, block_x_size, block_y_size, false, box_border_mode, roi);
	}

	float MinEigenValue(const float S_xx, const float S_xy, const float S_yy, const float normalization)
	{
		const float d = std::max((S_xx - S_yy) * (S_xx - S_yy) + 4.0f * S_xy * S_xy, 0.0f);
		const float lam_min = (S_xx + S_yy - std::sqrt(d)) * 0.5f;
		return normalization > 0.0f ? lam_min / normalization : lam_min;
	}

	float HarrisResponse(const float S_xx, const float S_yy, const float S_xy, const float k, const float normalization)
	{
		const float det = S_xx * S_yy - S_xy * S_xy;
		const float trace = S_xx + S_yy;
		return (det - k * trace * trace) * normalization;
	}

	float CornerResponse(const float S_xx, const float S_yy, const float S_xy, const float k, const float normalization, const bool is_harris)
	{
		if (is_harris)
		{
			return HarrisResponse(S_xx, S_yy, S_xy, k, normalization);
		}
		else
		{
			return MinEigenValue(S_xx, S_xy, S_yy, normalization);
		}
	}

	Image<ImageFormat::GRAY, float> CornerResponse(
		const Image<ImageFormat::GRAY, float>& S_xx,
		const Image<ImageFormat::GRAY, float>& S_yy,
		const Image<ImageFormat::GRAY, float>& S_xy,
		const float k,
		const float normalization,
		const bool is_harris,
		const Rectangle<int>& roi)
	{
		// validate that the images have the same dimensions
		assert(S_xx.Width() == S_yy.Width() && S_xx.Height() == S_yy.Height());
		assert(S_xx.Width() == S_xy.Width() && S_xx.Height() == S_xy.Height());

		Image<ImageFormat::GRAY, float> response;
		response.Create(S_xx.Width(), S_xx.Height(), qlm::Pixel<ImageFormat::GRAY, float>{0.0f});

		auto [start_x, start_y, end_x, end_y] = ROI(roi, S_xx.Width(), S_xx.Height());

		for (int y = start_y; y < end_y; y++)
		{
			for (int x = start_x; x < end_x; x++)
			{
				const float ixx = S_xx.GetPixel(x, y).v;
				const float iyy = S_yy.GetPixel(x, y).v;
				const float ixy = S_xy.GetPixel(x, y).v;

				const float response_value = CornerResponse(ixx, iyy, ixy, k, normalization, is_harris);

				response.SetPixel(x, y, response_value);
			}
		}

		return response;
	}
}

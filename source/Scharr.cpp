#define _USE_MATH_DEFINES

#include "Scharr.hpp"
#include "SepFilter2D.hpp"
#include <cmath>


namespace qlm
{
	// change scharr image from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertScharrDepth(Image < ImageFormat::GRAY, int16_t>& in)
	{
		constexpr uint8_t max_value = std::numeric_limits<uint8_t>::max();
		constexpr uint8_t min_value = std::numeric_limits<uint8_t>::lowest();

		Image<ImageFormat::GRAY, uint8_t> out{ in.width, in.height };
		Pixel<ImageFormat::GRAY, uint8_t> out_pix;

		// max-min values
		const auto ker = GetScharrYKernel();
		const int min = 0;
		const int max = max_value * (ker.x_ker.Get(0) + ker.x_ker.Get(1) + ker.x_ker.Get(2));

		float v{ 0 }, a{ 0 };

		for (int i = 0; i < out.height * out.width; i++)
		{
			// remove sign
			v = std::abs(in.GetPixel(i).v);
			a = std::abs(in.GetPixel(i).a);

			// normalize value
			v = (v - min) / (float)(max - min);
			a = (a - min) / (float)(max - min);

			// denormalize
			out_pix.v = static_cast<uint8_t>(v * (max_value - min_value) + min_value);
			out_pix.a = static_cast<uint8_t>(a * (max_value - min_value) + min_value);

			out.SetPixel(i, out_pix);
		}

		return out;
	}
	
	// generate coefficients for Scharr X
	SepKernel GetScharrXKernel()
	{
		SepKernel out(3, 3);
		// x ker : derivative kernel [-1, 0, 1]
		out.x_ker.Set(0, -1);
		out.x_ker.Set(1, 0);
		out.x_ker.Set(2, 1);
		// y ker : smoothing kernel [3, 10, 3]
		out.y_ker.Set(0, 3);
		out.y_ker.Set(1, 10);
		out.y_ker.Set(2, 3);
		return out;
	}
	
	// generate coefficients for Scharr Y
	SepKernel GetScharrYKernel()
	{
		SepKernel out(3, 3);
		// x ker : smoothing kernel [3, 10, 3]
		out.x_ker.Set(0, 3);
		out.x_ker.Set(1, 10);
		out.x_ker.Set(2, 3);
		// y ker : derivative kernel [-1, 0, 1]
		out.y_ker.Set(0, -1);
		out.y_ker.Set(1, 0);
		out.y_ker.Set(2, 1);
		return out;
	}
	
	// Scharr X operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> ScharrX(const Image<ImageFormat::GRAY, in_t>& in, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		const auto ker = GetScharrXKernel();

		return SepFilter2D<ImageFormat::GRAY, in_t, out_t>(in, ker, border_mode);
	}

	// Scharr Y operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> ScharrY(const Image<ImageFormat::GRAY, in_t>& in, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		const auto ker = GetScharrYKernel();

		return SepFilter2D<ImageFormat::GRAY, in_t, out_t>(in, ker, border_mode);
	}

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ScharrX<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ScharrY<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);
}	
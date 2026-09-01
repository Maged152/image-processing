#define _USE_MATH_DEFINES

#include "Scharr.hpp"
#include "SepFilter2D.hpp"
#include <iostream>
#include <cmath>
#include <vector>


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

	// Scharr operation
	template<pixel_t in_t, pixel_t out_t>
	ScharrDerivatives<in_t, out_t> Scharr(const Image<ImageFormat::GRAY, in_t>& in, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		const in_t min_value = std::numeric_limits<in_t>::lowest();
		const in_t max_value = std::numeric_limits<in_t>::max();

		const wider_t<in_t> max_mag = std::sqrt(std::pow(max_value, 2) + std::pow(max_value, 2));

		ScharrDerivatives<in_t, out_t> out{ in.width, in.height };

		// x derivative
		out.scharr_x = std::move(ScharrX<in_t, out_t>(in, border_mode));

		// y derivative
		out.scharr_y = std::move(ScharrY<in_t, out_t>(in, border_mode));

		// scharr x y as in_t(uint8_t)
		auto scharr_x = ConvertScharrDepth(out.scharr_x);
		auto scharr_y = ConvertScharrDepth(out.scharr_y);

		Pixel<ImageFormat::GRAY, in_t> x_pix;
		Pixel<ImageFormat::GRAY, in_t> y_pix;
		Pixel<ImageFormat::GRAY, in_t> out_pix;
		// magnitude
		float v{ 0 }, a{ 0 }, angle{ 0 };
		for (int i = 0; i < in.height * in.width; i++)
		{
			// input X, Y pixels
			x_pix = scharr_x.GetPixel(i);
			y_pix = scharr_y.GetPixel(i);

			// calculate the magnitude
			v = std::sqrt(std::pow(x_pix.v, 2) + std::pow(y_pix.v, 2));
			a = std::sqrt(std::pow(x_pix.a, 2) + std::pow(y_pix.a, 2));

			// normalize value
			v = (v - min_value) / (float)(max_mag - min_value);
			a = (a - min_value) / (float)(max_mag - min_value);

			// denormalize
			out_pix.v = static_cast<in_t>(v * (max_value - min_value) + min_value);
			out_pix.a = static_cast<in_t>(a * (max_value - min_value) + min_value);
			out.magnitude.SetPixel(i, out_pix);

			// calculate the angle
			angle = std::atan2(out.scharr_y.GetPixel(i).v, out.scharr_x.GetPixel(i).v) * 180.0f / M_PI;
			out.angle.SetPixel(i, Pixel<ImageFormat::GRAY, float>(angle));
		}

		return out;
	}
}

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ScharrX<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ScharrY<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);


// Explicit instantiation for  uint8_t, int16_t
template qlm::ScharrDerivatives<uint8_t, int16_t>
   qlm::Scharr<uint8_t, int16_t>(
	const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);
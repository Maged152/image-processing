#define _USE_MATH_DEFINES

#include "Sobel/Sobel.h"
#include "SepFilter2D/SepFilter2D.h"
#include <iostream>
#include <cmath>
#include <vector>


namespace qlm
{
	// change sobel image from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(Image < ImageFormat::GRAY, int16_t>& in, unsigned int filter_size)
	{
		constexpr uint8_t max_value = std::numeric_limits<uint8_t>::max();
		constexpr uint8_t min_value = std::numeric_limits<uint8_t>::lowest();
		// out
		Image<ImageFormat::GRAY, uint8_t> out{in.Width(), in.Height()};
		Pixel<ImageFormat::GRAY, uint8_t> out_pix;
		// get filter 
		auto ker = GetDerivYKernel(filter_size);
		// max-min values
		int min{ 0 }, max{ 0 };
		for (int i = 0; i < ker.x_ker.Length(); i++)
		{
			max += max_value * ker.x_ker.Get(i);
		}
		
		float v{ 0 }, a{ 0 };

		for (int i = 0; i < out.Height() * out.Width(); i++)
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
	// generate derivative kernel
	Kernel1D GetDerivKernel(unsigned int n, int order)
	{
		Kernel1D out{ n };
		std::vector<int> ker(n + 1, 0);
		ker[0] = 1;

		int old_val, new_val;

		for (int i = 0; i < n - order - 1; i++)
		{
			old_val = ker[0];
			for (int j = 1; j <= n; j++)
			{
				new_val = ker[j] + ker[j - 1];
				ker[j - 1] = old_val;
				old_val = new_val;
			}
		}

		for (int i = 0; i < order; i++)
		{
			old_val = -ker[0];
			for (int j = 1; j <= n; j++)
			{
				new_val = ker[j - 1] - ker[j];
				ker[j - 1] = old_val;
				old_val = new_val;
			}
		}
		// store kernel
		for (int i = 0; i < n; i++)
		{
			out.Set(i, ker[i]);
		}

		return out;
	}
	// generate coefficients for Sobel X
	SepKernel GetDerivXKernel(unsigned int n)
	{
		SepKernel out(n);
		// check kernel size
		if (n % 2 == 0 || n < 3)
		{
			std::cout << "The kernel size must be odd & greater than 1\n";
			return out;
		}
		// x ker
		out.x_ker = GetDerivKernel(n, 1);
		// y ker
		out.y_ker = GetDerivKernel(n, 0);
		return out;
	}
	// generate coefficients for Sobel Y
	SepKernel GetDerivYKernel(unsigned int n)
	{
		SepKernel out(n);
		// check kernel size
		if (n % 2 == 0 || n < 3)
		{
			std::cout << "The kernel size must be odd & greater than 1\n";
			return out;
		}
		// x ker
		out.x_ker = GetDerivKernel(n, 0);
		// y ker
		out.y_ker = GetDerivKernel(n, 1);
		return out;
	}
	// Sobel X operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> SobelX(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		auto ker = GetDerivXKernel(kernel_size);
		return SepFilter2D<ImageFormat::GRAY, in_t, out_t>(in, ker.x_ker, ker.y_ker, border_mode);
	}
	// Sobel Y operation
	template<pixel_t in_t, pixel_t out_t>
	Image<ImageFormat::GRAY, out_t> SobelY(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		auto ker = GetDerivYKernel(kernel_size);
		return SepFilter2D<ImageFormat::GRAY, in_t, out_t>(in, ker.x_ker, ker.y_ker, border_mode);
	}
	// Sobel operation
	template<pixel_t in_t, pixel_t out_t>
	SobelDerivatives<in_t, out_t> Sobel(const Image<ImageFormat::GRAY, in_t>& in, const unsigned int kernel_size, const BorderMode<ImageFormat::GRAY, in_t>& border_mode)
	{
		const in_t min_value = std::numeric_limits<in_t>::lowest();
		const in_t max_value = std::numeric_limits<in_t>::max();

		const wider_t<in_t> max_mag = std::sqrtf(std::powf(max_value, 2) + std::powf(max_value, 2));

		SobelDerivatives<in_t, out_t> out {in.Width(), in.Height()};
		// x derivative
		out.sobel_x = std::move(SobelX<in_t, out_t>(in, kernel_size, border_mode));
		// y derivative
		out.sobel_y = std::move(SobelY<in_t, out_t>(in, kernel_size, border_mode));
		// sobel x y as in_t(uint8_t)
		auto sobel_x = ConvertSobelDepth(out.sobel_x, kernel_size);
		auto sobel_y = ConvertSobelDepth(out.sobel_y, kernel_size);

		Pixel<ImageFormat::GRAY, in_t> x_pix;
		Pixel<ImageFormat::GRAY, in_t> y_pix;
		Pixel<ImageFormat::GRAY, in_t> out_pix;
		// magnitude
		float v{0}, a{0}, angle{0};
		for (int i = 0; i < in.Height() * in.Width(); i++)
		{
			// input X, Y pixels
			x_pix = sobel_x.GetPixel(i);
			y_pix = sobel_y.GetPixel(i);
			// calculate the magnitude
			v = std::sqrtf(std::powf(x_pix.v, 2) + std::powf(y_pix.v, 2));
			a = std::sqrtf(std::powf(x_pix.a, 2) + std::powf(y_pix.a, 2));
			// normalize value
			v = (v - min_value) / (float)(max_mag - min_value);
			a = (a - min_value) / (float)(max_mag - min_value);
			// denormalize
			out_pix.v = static_cast<in_t>(v * (max_value - min_value) + min_value);
			out_pix.a = static_cast<in_t>(a * (max_value - min_value) + min_value);
			// store
			out.magnitude.SetPixel(i, out_pix);
			// calculate the angle
			angle = std::atan2f(out.sobel_y.GetPixel(i).v, out.sobel_x.GetPixel(i).v) * 180.0f / M_PI;
			out.angle[i] = angle;
		}

		return out;
	}
}

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::SobelX<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	unsigned int,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);

// Explicit instantiation for  uint8_t, int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::SobelY<uint8_t, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	unsigned int,
	const BorderMode<ImageFormat::GRAY, uint8_t>&);


// Explicit instantiation for  uint8_t, int16_t
template qlm::SobelDerivatives<uint8_t, int16_t>
   qlm::Sobel<uint8_t, int16_t>(
	const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const unsigned int,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);

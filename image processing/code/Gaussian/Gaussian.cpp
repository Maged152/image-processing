#include "Gaussian/Gaussian.h"
#include "SepFilter2D/SepFilter2D.h"
#include <cmath>
namespace qlm
{
	Kernel1D GetGaussianKernel(const unsigned int filter_size, float sigma)
	{
		Kernel1D out{ filter_size };

		float sum{ 0 };
		float gaussian_value{ 0 };
		float center = (filter_size - 1) * 0.5;

		if (sigma <= 0)
		{
			sigma = 0.3 * (center - 1) + 0.8;
		}

		for (int i = 0; i < filter_size; i++)
		{
			gaussian_value = std::exp(-1 * std::pow(i - center, 2) / (2 * sigma * sigma));
			sum += gaussian_value;
			out.Set(i, gaussian_value);
		}
		// Normalize the kernel
		for (int i = 0; i < filter_size; i++)
		{
			gaussian_value = out.Get(i);
			gaussian_value /= sum;
			out.Set(i, gaussian_value);
		}

		return out;
	}

	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Gaussian(const Image<frmt, T>& in, const unsigned int filter_size, float sigma_x, float sigma_y, const  BorderMode<frmt, T>& border_mode)
	{
		sigma_y = sigma_y == 0 ? sigma_x : sigma_y;

		auto ker_x = GetGaussianKernel(filter_size, sigma_x);
		auto ker_y = GetGaussianKernel(filter_size, sigma_y);

		return SepFilter2D<frmt, T, T>(in, ker_x, ker_y, border_mode);
	}
}

// Explicit instantiation for RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::Gaussian<qlm::ImageFormat::RGB, uint8_t>(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&,
	const unsigned int,
	float,
	float,
	const BorderMode<qlm::ImageFormat::RGB, uint8_t>&);
// Explicit instantiation for RGB , int16_t
template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::Gaussian<qlm::ImageFormat::RGB, int16_t>(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&,
	const unsigned int,
	float,
	float,
	const BorderMode<qlm::ImageFormat::RGB, int16_t>&);
// Explicit instantiation for GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::Gaussian<qlm::ImageFormat::GRAY, uint8_t>(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&,
	const unsigned int,
	float,
	float,
	const BorderMode<qlm::ImageFormat::GRAY, uint8_t>&);
// Explicit instantiation for GRAY , int16_t
template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::Gaussian<qlm::ImageFormat::GRAY, int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&,
	const unsigned int,
	float,
	float,
	const BorderMode<qlm::ImageFormat::GRAY, int16_t>&);
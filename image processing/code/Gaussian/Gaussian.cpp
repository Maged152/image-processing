#include "Gaussian/Gaussian.h"
#include <cmath>
namespace qlm
{
	Kernel1D GetGaussianKernel(unsigned int filter_size, double sigma)
	{
		Kernel1D out{ filter_size };

		double sum{ 0 };
		double gaussian_value{ 0 };
		double center = (filter_size - 1) * 0.5;

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
	Image<frmt, T> Gaussian(const Image<frmt, T>& in, unsigned int filter_size, double sigma_x, double sigma_y, int border_value)
	{
		return Image<frmt, T>();
	}
}
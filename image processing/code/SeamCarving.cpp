#include "SeamCarving.h"
#include "Sobel.h"
#include "ColorConvert.h"
#include <iostream>
#include <cmath>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(const Image<frmt, T>& in, 
		const size_t width, const size_t height,
		const EnergyFlag energy, const OrderFlag order)
	{
		auto gray = ColorConvert<frmt, T, ImageFormat::GRAY, T>(in);

		Image<ImageFormat::GRAY, int16_t> energy_map;

		if (energy == EnergyFlag::BACKWARD)
		{
			auto sobelx = SobelX(gray, 3);
			energy_map = SobelY(gray, 3);

			for (int i = 0; i < energy_map.Width() * energy_map.Height(); i++)
			{
				int16_t grad_mag = std::abs(energy_map.GetPixel(i).v) + std::abs(sobelx.GetPixel(i).v);
				energy_map.SetPixel(i, grad_mag);
			}
		}

		const BorderMode<ImageFormat::GRAY, int16_t> border_mode = { 
			.border_type = BorderType::BORDER_CONSTANT,
			.border_pixel = Pixel<ImageFormat::GRAY, int16_t>{ std::numeric_limits<int16_t>::max() }
		};
		
		// populate DP matrix
		for (int y = 1; y < energy_map.Height(); y++)
		{
			for (int x = 0; x < energy_map.Width(); x++)
			{
				int16_t min_energy = std::numeric_limits<int16_t>::max();
				for (int i = -1; i < 2; i++)
				{
					min_energy = std::min(min_energy, energy_map.GetPixel(x + i, y - 1, border_mode).v);
				}
				energy_map.SetPixel(x, y, min_energy + energy_map.GetPixel(x, y).v);
			}
		}

		// find optimal seam
		int min_index{ 0 };
		int16_t min_val = energy_map.GetPixel(0, energy_map.Height() - 1).v;

		for (int i = 1; i < energy_map.Width(); i++)
		{
			if (min_val > energy_map.GetPixel(i, energy_map.Height() - 1).v)
			{
				min_val = energy_map.GetPixel(i, energy_map.Height() - 1).v;
				min_index = i;
			}
		}

		Image<frmt, T> out = in;
		const Pixel<frmt, T> red{255, 0, 0};

		out.SetPixel(min_index, energy_map.Height() - 1, red);

		for (int y = energy_map.Height() - 2; y > -1; y--)
		{
			min_val = std::numeric_limits<int16_t>::max();
			int next_index;
			for (int i = -1; i < 2; i++)
			{
				if (min_val > energy_map.GetPixel(min_index + i, y, border_mode).v)
				{
					min_val = energy_map.GetPixel(min_index + i, y, border_mode).v;
					next_index = min_index + i;
				}
			}
			min_index = next_index;

			out.SetPixel(min_index, y, red);
		}

		return out;
	}

	template Image<ImageFormat::RGB, uint8_t> SeamCarving(const Image<ImageFormat::RGB, uint8_t>&,
		const size_t, const size_t,
		const EnergyFlag, const OrderFlag);
}
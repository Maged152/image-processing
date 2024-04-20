#include "SeamCarving.h"
#include "Sobel.h"
#include "ColorConvert.h"
#include <iostream>
#include <cmath>

namespace qlm
{
	struct ValueIndex
	{
		int index;
		int32_t value;

		// Overload operator< to compare based on the value member
		bool operator<(const ValueIndex& other) const 
		{
			return value < other.value;
		}
	};

	void PrintImage(const Image<ImageFormat::GRAY, int32_t>& image)
	{
		Image<ImageFormat::GRAY, uint8_t> out{ image.width, image.height };

		int32_t min_pix{ image.GetPixel(0).v };
		int32_t max_pix = min_pix;

		for (int y = 0; y < image.height; y++)
		{
			for (int x = 0; x < image.width; x++)
			{
				auto pix = image.GetPixel(x, y).v;
				if (pix > max_pix) max_pix = pix;
				else if (pix < min_pix) min_pix = pix;
			}
		}

		for (int y = 0; y < image.height; y++)
		{
			for (int x = 0; x < image.width; x++)
			{
				uint8_t pix = ((image.GetPixel(x, y).v - min_pix) / (float)(max_pix - min_pix)) * 255;
				out.SetPixel(x, y, pix);
			}
		}

		std::cout << max_pix << "  "  << min_pix << "\n";
		out.SaveToFile("tem0.jpg");
	}

	template<bool vertical, ImageFormat frmt, pixel_t T>
	void RemovePixel(const int x, const int y, Image<frmt, T>& out)
	{
		if constexpr (vertical)
		{
			// shift left
			for (int i = x; i < out.width - 1; i++)
			{
				out.SetPixel(i, y, out.GetPixel(i + 1, y));
			}
		}
		else
		{
			// shift up
			for (int i = y; i < out.height - 1; i++)
			{
				out.SetPixel(x, i, out.GetPixel(x, i + 1));
			}
		}
	}

	template<ImageFormat frmt, pixel_t T>
	void GetAspectRatio(const size_t width, const size_t height, const Image<frmt, T>& in, 
		                      size_t& dx, size_t& dy, bool& dec_x, bool& dec_y)
	{
		if (width < in.width)
		{
			dx = in.width - width;
			dec_x = true;
		}
		else
		{
			dx = width - in.width;
			dec_x = false;
		}

		if (height < in.height)
		{
			dy = in.height - height;
			dec_y = true;
		}
		else
		{
			dy = height - in.height;
			dec_y = false;
		}
	}

	template<ImageFormat frmt, pixel_t T>
	void RemoveSeam(const Image<ImageFormat::GRAY, int32_t>& energy_map, Image<frmt, T>& out, Image<ImageFormat::GRAY, T>& gray)
	{
		// find optimal seam
		int x_idx{ 0 };
		int32_t min_val = energy_map.GetPixel(x_idx, energy_map.height - 1).v;

		for (int x = 1; x < energy_map.width; x++)
		{
			if (min_val > energy_map.GetPixel(x, energy_map.height - 1).v)
			{
				min_val = energy_map.GetPixel(x, energy_map.height - 1).v;
				x_idx = x;
			}
		}

		// remove the pixel
		RemovePixel<true>(x_idx, energy_map.height - 1, out);
		RemovePixel<true>(x_idx, energy_map.height - 1, gray);
		
		
		const BorderMode<ImageFormat::GRAY, int32_t> border_mode = {
			.border_type = BorderType::BORDER_CONSTANT,
			.border_pixel = Pixel<ImageFormat::GRAY, int32_t>{ std::numeric_limits<int32_t>::max() }
		};

		for (int y = energy_map.height - 2; y >= 0; y--)
		{
			ValueIndex left, right, mid;

			left.index = std::max(x_idx - 1, 0);
			mid.index = x_idx;
			right.index = std::min(x_idx + 1, (int)energy_map.width - 1);

			left.value = energy_map.GetPixel(left.index, y - 1).v;
			mid.value = energy_map.GetPixel(mid.index, y - 1).v;
			right.value = energy_map.GetPixel(right.index, y - 1).v;

			const ValueIndex min_energy = std::min(left, std::min(mid, right));

			x_idx = min_energy.index;

			// remove the pixel
			RemovePixel<true>(x_idx, y, out);
			RemovePixel<true>(x_idx, y, gray);
		}
	}

	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(const Image<frmt, T>& in, 
		const size_t width, const size_t height,
		const EnergyFlag energy, const OrderFlag order)
	{
		Image<frmt, T> out{ width , height };

		// buffers used in the algorithm
		Image<frmt, T> temp = in;
		Image<ImageFormat::GRAY, T> gray = ColorConvert<frmt, T, ImageFormat::GRAY, T>(in);
		Image<ImageFormat::GRAY, int32_t> energy_map {in.width, in.height};
		// how much to remove/insert
		size_t dx, dy;
		bool dec_x, dec_y;
		GetAspectRatio(width, height, in, dx, dy, dec_x, dec_y);

		const BorderMode<ImageFormat::GRAY, int32_t> border_mode = {
			.border_type = BorderType::BORDER_CONSTANT,
			.border_pixel = Pixel<ImageFormat::GRAY, int32_t>{ std::numeric_limits<int32_t>::max() }
		};

		// remove from the width
		for (int iter = 0; iter < dx; iter++)
		{
			// compute the energy 
			if (energy == EnergyFlag::BACKWARD)
			{
				Image<ImageFormat::GRAY, int16_t> sobelx = SobelX(gray, 3);
				Image<ImageFormat::GRAY, int16_t> sobely = SobelY(gray, 3);

				for (int y = 0; y < sobelx.height; y++)
				{
					for (int x = 0; x < sobelx.width; x++)
					{
						int32_t grad_mag = std::abs(sobelx.GetPixel(x, y).v) + std::abs(sobely.GetPixel(x, y).v);
						energy_map.SetPixel(x, y, grad_mag);
					}
				}
			}
			
			// populate DP matrix
			for (int y = 1; y < energy_map.height; y++)
			{
				for (int x = 0; x < energy_map.width; x++)
				{
					int32_t left = energy_map.GetPixel(std::max(x - 1, 0), y - 1).v;
					int32_t mid = energy_map.GetPixel(x, y - 1).v;
					int32_t right = energy_map.GetPixel(std::min(x + 1, (int)energy_map.width - 1), y - 1).v;

					int32_t min_energy = std::min(left, std::min(mid, right));

					energy_map.SetPixel(x, y, min_energy + energy_map.GetPixel(x, y).v);
				}
			}

			//PrintImage(energy_map);
			// find & remove optimal seam
			RemoveSeam(energy_map, temp, gray);
				
			gray.width--;
			energy_map.width--;
		}
		
		for (int y = 0; y < out.height; y++)
		{
			for (int x = 0; x < out.width; x++)
			{
				out.SetPixel(x, y, temp.GetPixel(x, y));
			}
		}

		return out;
	}

	template Image<ImageFormat::RGB, uint8_t> SeamCarving(const Image<ImageFormat::RGB, uint8_t>&,
		const size_t, const size_t,
		const EnergyFlag, const OrderFlag);
}
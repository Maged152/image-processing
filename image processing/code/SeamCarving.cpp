#include "SeamCarving.h"
#include "Sobel.h"
#include "ColorConvert.h"
#include "Transposeh.h"
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

	void AllocMem(const size_t in_width, const size_t in_height, 
		          const bool dec_x, const bool dec_y,
		          const size_t dx, const size_t dy, const OrderFlag order,
		          Image<ImageFormat::GRAY, int32_t>& energy_map)
	{
		size_t d0, d1;
		if (dec_x && dec_y)
		{
			d0 = std::max(in_width, in_height);

			if (order == OrderFlag::WIDTH_FIRST)
			{
				d1 = std::max(in_height, in_width - dx);
			}
			else
			{
				d1 = std::max(in_height - dy, in_width);
			}
		}
		else if (dec_x)
		{
			d0 = in_width;
			d1 = in_height;
		}
		else
		{
			d0 = in_height;
			d1 = in_width;
		}

		energy_map.create(d0, d1);
	}

	template<ImageFormat frmt, pixel_t T>
	void RemoveVerticalSeam(const Image<ImageFormat::GRAY, int32_t>& energy_map, Image<frmt, T>& out, Image<ImageFormat::GRAY, T>& gray)
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

		for (int y = energy_map.height - 2; y >= 0; y--)
		{
			ValueIndex left, right, mid;

			left.index = std::max(x_idx - 1, 0);
			mid.index = x_idx;
			right.index = std::min(x_idx + 1, (int)energy_map.width - 1);

			left.value = energy_map.GetPixel(left.index, y).v;
			mid.value = energy_map.GetPixel(mid.index, y).v;
			right.value = energy_map.GetPixel(right.index, y).v;

			const ValueIndex min_energy = std::min(left, std::min(mid, right));

			x_idx = min_energy.index;

			// remove the pixel
			RemovePixel<true>(x_idx, y, out);
			RemovePixel<true>(x_idx, y, gray);
		}
	}

	
	template<pixel_t T>
	void EnergyFunction(const Image<ImageFormat::GRAY, T>& gray, Image<ImageFormat::GRAY, int32_t>& energy_map)
	{
		Image<ImageFormat::GRAY, int16_t> sobelx = SobelX(gray, 3);
		Image<ImageFormat::GRAY, int16_t> sobely = SobelY(gray, 3);

		for (int y = 0; y < sobelx.height; y++)
		{
			for (int x = 0; x < sobelx.width; x++)
			{
				const int32_t grad_mag = std::abs(sobelx.GetPixel(x, y).v) + std::abs(sobely.GetPixel(x, y).v);
				energy_map.SetPixel(x, y, grad_mag);
			}
		}
	}

	void BackwardComulative(Image<ImageFormat::GRAY, int32_t>& energy_map)
	{
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
	}
	
	template<pixel_t T>
	void ForwardComulative(const Image<ImageFormat::GRAY, T>& gray, Image<ImageFormat::GRAY, int32_t>& energy_map)
	{
		// first row
		for (int x = 0; x < gray.width; x++)
		{
			constexpr int y = 0;
			const int left_x = std::max(0, x - 1);
			const int right_x = std::min((int)gray.width - 1, x + 1);

			const int32_t pix_left = gray.GetPixel(left_x, y).v;
			const int32_t pix_right = gray.GetPixel(right_x, y).v;

			const int32_t cost = std::abs(pix_left - pix_right);

			energy_map.SetPixel(x, 0, cost + energy_map.GetPixel(x, y).v);
		}

		for (int y = 1; y < gray.height; y++)
		{
			for (int x = 0; x < gray.width; x++)
			{
				const int left_x = std::max(0, x - 1);
				const int right_x = std::min((int)gray.width - 1, x + 1);

				const int32_t pix_left = gray.GetPixel(left_x, y).v;
				const int32_t pix_right = gray.GetPixel(right_x, y).v;
				const int32_t pix_top = gray.GetPixel(x, y - 1).v;

				const int32_t cost_u = std::abs(pix_left - pix_right);
				const int32_t cost_l = cost_u + std::abs(pix_left - pix_top);
				const int32_t cost_r = cost_u + std::abs(pix_right - pix_top);

				ValueIndex m_u, m_l, m_r;

				m_u.value = cost_u + energy_map.GetPixel(x, y - 1).v;
				m_l.value = cost_l + energy_map.GetPixel(left_x, y - 1).v;
				m_r.value = cost_r + energy_map.GetPixel(right_x, y - 1).v;

				const ValueIndex min_cost = std::min(m_u, std::min(m_l, m_r));

				energy_map.SetPixel(x, y, min_cost.value + energy_map.GetPixel(x, y).v);
			}
		}
	}

	template<ImageFormat frmt, pixel_t T>
	void ReduceWidth(const int dx, const EnergyFlag energy, Image<ImageFormat::GRAY, T>& gray, Image<ImageFormat::GRAY, int32_t>& energy_map, Image<frmt, T>& temp)
	{
		for (int iter = 0; iter < dx; iter++)
		{
			// compute the energy
			EnergyFunction(gray, energy_map);

			// populate DP matrix
			if (energy == EnergyFlag::BACKWARD)
			{
				BackwardComulative(energy_map); 
			}
			else
			{
				ForwardComulative(gray, energy_map);
			}


			// find & remove optimal seam
			RemoveVerticalSeam(energy_map, temp, gray);

			gray.width--;
			energy_map.width--;
			temp.width--;
		}
	}

	template<ImageFormat frmt, pixel_t T>
	void ReduceHeight(const int dy, const EnergyFlag energy, Image<ImageFormat::GRAY, T>& gray, Image<ImageFormat::GRAY, int32_t>& energy_map, Image<frmt, T>& temp)
	{
		auto gray_t = Transpose(gray);
		energy_map.width = gray_t.width;
		energy_map.height = gray_t.height;

		auto temp_t = Transpose(temp);
		ReduceWidth(dy, energy, gray_t, energy_map, temp_t);

		temp = Transpose(temp_t);
		gray = Transpose(gray_t);
	}

	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> SeamCarving(const Image<frmt, T>& in, 
		const size_t width, const size_t height,
		const EnergyFlag energy, const OrderFlag order)
	{
		Image<frmt, T> out{ width , height };

		// buffer used in the algorithm
		Image<ImageFormat::GRAY, T> gray = ColorConvert<frmt, T, ImageFormat::GRAY, T>(in);
		Image<ImageFormat::GRAY, int32_t> energy_map;
		Image<frmt, T> temp = in;
		
		// how much to remove/insert
		size_t dx, dy;
		bool dec_x, dec_y;
		GetAspectRatio(width, height, in, dx, dy, dec_x, dec_y);

		// allocate proper memory for energy_map
		AllocMem(in.width, in.height, dec_x, dec_y, dx, dy, order, energy_map);
	
		if (order == OrderFlag::WIDTH_FIRST)
		{
			if (dec_x)
			{
				energy_map.width = gray.width;
				energy_map.height = gray.height;
				ReduceWidth(dx, energy, gray, energy_map, temp);
			}

			if (dec_y)
			{
				ReduceHeight(dy, energy, gray, energy_map, temp);
			}
				
		}
		else
		{
			if (dec_y)
			{
				ReduceHeight(dy, energy, gray, energy_map, temp);
			}

			if (dec_x)
			{
				energy_map.width = gray.width;
				energy_map.height = gray.height;
				ReduceWidth(dx, energy, gray, energy_map, temp);
			}
		}
		
		out.Copy(temp);

		return out;
	}

	template Image<ImageFormat::RGB, uint8_t> SeamCarving(const Image<ImageFormat::RGB, uint8_t>&,
		const size_t, const size_t,
		const EnergyFlag, const OrderFlag);
}
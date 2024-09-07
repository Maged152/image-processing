#include "common/image.hpp"
#include <algorithm>

namespace qlm
{
	// create image
	template<ImageFormat frmt, pixel_t T>
	void Image<frmt, T>::create(size_t img_width, size_t img_height, Pixel<frmt, T> pix, size_t img_stride)
	{
		width = img_width;
		height = img_height;
		stride = img_stride == 0 ? width : img_stride;

		if constexpr (frmt == ImageFormat::GRAY)
		{
			num_of_channels = 2;
		}
		else
		{
			num_of_channels = 4;
		}
		if (data != nullptr)
			delete[] data;
		data = new Pixel<frmt, T>[stride * height];

		std::fill_n(data, stride * height, pix);
	}

	template<ImageFormat frmt, pixel_t T>
	void Image<frmt, T>::create(size_t img_width, size_t img_height, size_t img_stride)
	{
		width = img_width;
		height = img_height;
		stride = img_stride == 0 ? width : img_stride;

		if constexpr (frmt == ImageFormat::GRAY)
		{
			num_of_channels = 2;
		}
		else
		{
			num_of_channels = 4;
		}
		if (data != nullptr)
			delete[] data;
		data = new Pixel<frmt, T>[stride * height];
	}

	// Get pixel
	template<ImageFormat frmt, pixel_t T>
	Pixel<frmt, T> Image<frmt, T>::GetPixel(int x, int y, const BorderMode<frmt, T>& border_mode) const
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
		{
			// not border
			return this->GetPixel(x, y);
		}
		else
		{
			// border pixel
			switch (border_mode.border_type)
			{
				case qlm::BorderType::BORDER_CONSTANT:
				{
					return border_mode.border_pixel;
				}
				case qlm::BorderType::BORDER_REPLICATE:
				{
					int x_idx = std::clamp(x, 0, static_cast<int>(width) - 1);
					int y_idx = std::clamp(y, 0, static_cast<int>(height) - 1);
					return this->GetPixel(x_idx, y_idx);
				}
				case qlm::BorderType::BORDER_REFLECT:
				{
					int x_idx = this->ReflectBorderIndex(x, width);
					int y_idx = this->ReflectBorderIndex(y, height);
					return this->GetPixel(x_idx, y_idx);
				}
				default:
				{
					return Pixel<frmt, T>{};
				}
			}
		}
	}


	// explicit instantiation
	template void Image<ImageFormat::GRAY, uint8_t>::create(size_t, size_t, Pixel<ImageFormat::GRAY, uint8_t>, size_t);
	template void Image<ImageFormat::GRAY, int16_t>::create(size_t, size_t, Pixel<ImageFormat::GRAY, int16_t>, size_t);
	template void Image<ImageFormat::GRAY, float>::create(size_t, size_t, Pixel<ImageFormat::GRAY, float>, size_t);

	template void Image<ImageFormat::RGB, uint8_t>::create(size_t, size_t, Pixel<ImageFormat::RGB, uint8_t>, size_t);
	template void Image<ImageFormat::RGB, int16_t>::create(size_t, size_t, Pixel<ImageFormat::RGB, int16_t>, size_t);
	template void Image<ImageFormat::RGB, float>::create(size_t, size_t, Pixel<ImageFormat::RGB, float>, size_t);

	template void Image<ImageFormat::HLS, uint8_t>::create(size_t, size_t, Pixel<ImageFormat::HLS, uint8_t>, size_t);
	template void Image<ImageFormat::HLS, int16_t>::create(size_t, size_t, Pixel<ImageFormat::HLS, int16_t>, size_t);
	template void Image<ImageFormat::HLS, float>::create(size_t, size_t, Pixel<ImageFormat::HLS, float>, size_t);

	template void Image<ImageFormat::HSV, uint8_t>::create(size_t, size_t, Pixel<ImageFormat::HSV, uint8_t>, size_t);
	template void Image<ImageFormat::HSV, int16_t>::create(size_t, size_t, Pixel<ImageFormat::HSV, int16_t>, size_t);
	template void Image<ImageFormat::HSV, float>::create(size_t, size_t, Pixel<ImageFormat::HSV, float>, size_t);

	template void Image<ImageFormat::YCrCb, uint8_t>::create(size_t, size_t, Pixel<ImageFormat::YCrCb, uint8_t>, size_t);
	template void Image<ImageFormat::YCrCb, int16_t>::create(size_t, size_t, Pixel<ImageFormat::YCrCb, int16_t>, size_t);
	template void Image<ImageFormat::YCrCb, float>::create(size_t, size_t, Pixel<ImageFormat::YCrCb, float>, size_t);

	// -------------------------------------------------------------------------------------------------------------
	template void Image<ImageFormat::GRAY, uint8_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::GRAY, int16_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::GRAY, int32_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::GRAY, float>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::RGB, uint8_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::RGB, int16_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::RGB, float>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HLS, uint8_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HLS, int16_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HLS, float>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HSV, uint8_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HSV, int16_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::HSV, float>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::YCrCb, uint8_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::YCrCb, int16_t>::create(size_t, size_t, size_t);
	template void Image<ImageFormat::YCrCb, float>::create(size_t, size_t, size_t);
	// ------------------------------------------------------------------------------------------------------------
	template Pixel<ImageFormat::GRAY, uint8_t> Image<ImageFormat::GRAY, uint8_t>::GetPixel(int, int, const BorderMode<ImageFormat::GRAY, uint8_t>&) const;
	template Pixel<ImageFormat::GRAY, int16_t> Image<ImageFormat::GRAY, int16_t>::GetPixel(int, int, const BorderMode<ImageFormat::GRAY, int16_t>&) const;
	template Pixel<ImageFormat::GRAY, int32_t> Image<ImageFormat::GRAY, int32_t>::GetPixel(int, int, const BorderMode<ImageFormat::GRAY, int32_t>&) const;
	template Pixel<ImageFormat::GRAY, float> Image<ImageFormat::GRAY, float>::GetPixel(int, int, const BorderMode<ImageFormat::GRAY, float>&) const;
	template Pixel<ImageFormat::RGB, uint8_t> Image<ImageFormat::RGB, uint8_t>::GetPixel(int, int, const BorderMode<ImageFormat::RGB, uint8_t>&) const;
	template Pixel<ImageFormat::RGB, int16_t> Image<ImageFormat::RGB, int16_t>::GetPixel(int, int, const BorderMode<ImageFormat::RGB, int16_t>&) const;
	template Pixel<ImageFormat::RGB, float> Image<ImageFormat::RGB, float>::GetPixel(int, int, const BorderMode<ImageFormat::RGB, float>&) const;

	// -------------------------------------------------------------------------------------------------------------
}
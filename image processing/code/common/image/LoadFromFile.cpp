#include "common/image.h"
#include "stb_image.h"
#include <iostream>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	bool Image<frmt, T>::LoadFromFile(const std::string& file_name)
	{
		int w, h, n; // width, height, number of channels
		T* img_data{ nullptr }; //  pointer to the data


		if constexpr (std::is_same_v<T, uint8_t>) // U8
		{
			img_data = stbi_load(file_name.c_str(), &w, &h, &n, 0);
		}
		else if constexpr (std::is_same_v<T, int16_t>)  // S16
		{
			img_data = (T*)stbi_load_16(file_name.c_str(), &w, &h, &n, 0);
		}
		else // floating-point
		{
			img_data = (T*)stbi_loadf(file_name.c_str(), &w, &h, &n, 0);
		}

		if (img_data == nullptr)
		{
			std::cerr << "Error loading image file " << file_name << std::endl;
			return false;
		}

		width = w;
		height = h;
		num_of_channels = n;

		if (data != nullptr)
			delete[] data;

		data = new Pixel<frmt, T>[width * height];

		if constexpr (frmt == ImageFormat::GRAY)
		{
			if (n == 1 || n == 3)
			{
				for (size_t i = 0, idx = 0; i < width * height * n; i += n, idx++)
				{
					T v = img_data[i];
					data[idx].Set(v);
				}
			}
			else if (n == 2 || n == 4)
			{
				for (size_t i = 0, idx = 0; i < width * height * n; i += n, idx++)
				{
					T v1 = img_data[i];
					T v2 = img_data[i + n - 1];
					data[idx].Set(v1, v2);
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (n == 1)
			{
				for (size_t i = 0; i < width * height * n; i += n)
				{
					T v = img_data[i];
					data[i].Set(v, v, v);
				}
			}
			else if (n == 2)
			{
				for (size_t i = 0, idx = 0; i < width * height * n; i += n, idx++)
				{
					T v1 = img_data[i]; // value
					T v2 = img_data[i + 1]; // alpha
					data[idx].Set(v1, v1, v1, v2);
				}
			}
			else if (n == 3)
			{
				for (size_t i = 0, idx = 0; i < width * height * n; i += n, idx++)
				{
					T v1 = img_data[i];
					T v2 = img_data[i + 1];
					T v3 = img_data[i + 2];
					data[idx].Set(v1, v2, v3);
				}
			}
			else if (n == 4)
			{
				for (size_t i = 0, idx = 0; i < width * height * n; i += n, idx++)
				{
					T v1 = img_data[i];
					T v2 = img_data[i + 1];
					T v3 = img_data[i + 2];
					T v4 = img_data[i + 3];
					data[idx].Set(v1, v2, v3, v4);
				}
			}
			else
			{
				return false;
			}

		}

		stbi_image_free(img_data);
		return true;
	}


	template bool Image<ImageFormat::GRAY, uint8_t>::LoadFromFile(const std::string&);
	template bool Image<ImageFormat::RGB, uint8_t>::LoadFromFile(const std::string&);
	template bool Image<ImageFormat::GRAY, int16_t>::LoadFromFile(const std::string&);
	template bool Image<ImageFormat::RGB, int16_t>::LoadFromFile(const std::string&);
}


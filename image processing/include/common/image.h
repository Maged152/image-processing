#pragma once

#include "pixel.h"
#include <string>
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>
#include <algorithm>
#include <fstream>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	class Image
	{
	private:
		unsigned int width;
		unsigned int height;
		unsigned int num_of_channels;
		Pixel<frmt, T>* data;
	public:
		Image():data(nullptr), width(0), height(0)
		{
			if constexpr (frmt == ImageFormat::GRAY)
			{
				num_of_channels = 2;
			}
			else
			{
				num_of_channels = 4;
			}
		}

		Image(unsigned int width, unsigned int height) : width(width), height(height)
		{
			data = new Pixel<frmt, T>[width * height];
			if constexpr (frmt == ImageFormat::GRAY)
			{
				num_of_channels = 2;
			}
			else
			{
				num_of_channels = 4;
			}
		}

		~Image()
		{
			if (data != nullptr)
			{
				delete[] data;
				data = nullptr;
			}
			width = 0;
			height = 0;
		}

		Image(const Image<frmt, T>& other) : width(other.width), height(other.height), num_of_channels(other.num_of_channels)
		{
			data = new Pixel<frmt, T>[width * height];
			std::memcpy(data, other.data, (size_t)width * height * sizeof(Pixel<frmt, T>));
		}

		Image(Image<frmt, T>&& other) noexcept : width(other.width), height(other.height), num_of_channels(other.num_of_channels), data(other.data)
		{
			other.width = 0;
			other.height = 0;
			other.data = nullptr;
		}

		Image<frmt, T>& operator=(const Image<frmt, T>& other)
		{
			// Check for self-assignment
			if (this == &other)
				return *this;

			// Deallocate existing data
			if (data != nullptr)
				delete[] data;

			// Copy the other object's width ,height and number of channels
			width = other.width;
			height = other.height;
			num_of_channels = other.num_of_channels;
			// Allocate new memory and copy the other object's data
			data = new Pixel<frmt, T>[width * height];
			std::memcpy(data, other.data, (size_t)width * height * sizeof(Pixel<frmt, T>));

			return *this;
		}

		Image<frmt, T>& operator=(Image<frmt, T>&& other) noexcept
		{
			if (this != &other)
			{
				if (data != nullptr)
					delete[] data;

				width = other.width;
				height = other.height;
				data = other.data;
				num_of_channels = other.num_of_channels;

				other.width = 0;
				other.height = 0;
				other.data = nullptr;
			}
			return *this;
		}

	public:
		void create(unsigned int img_width, unsigned int img_height, Pixel<frmt, T> pix = Pixel<frmt, T>{})
		{
			width = img_width;
			height = img_height;
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
			data = new Pixel<frmt, T>[width * height];

			std::fill_n(data, width * height, pix);
		}
		
		void SetPixel(int x, int y, const Pixel<frmt, T> &pix)
		{
			if (x > 0 && x < width && y > 0 && y < height)
			{
				data[y * width + x] = pix;
			}
		}

		void SetPixel(int i, const Pixel<frmt, T>& pix)
		{
			if (i > 0 && i < width * height)
			{
				data[i] = pix;
			}	
		}

		Pixel<frmt, T> GetPixel(int x, int y) const
		{
			return data[y * width + x];
		}

		Pixel<frmt, T> GetPixel(int i) const
		{
			return data[i];
		}

		bool LoadFromFile(const std::string& file_name);

		bool SaveToFile(const std::string& file_name, bool alpha = true,int quality = 100);

		unsigned int Width() const
		{
			return width;
		}

		unsigned int Height() const
		{
			return height;
		}

		unsigned int NumerOfChannels() const
		{
			return num_of_channels;
		}
	};	
}


template<qlm::ImageFormat frmt, qlm::pixel_t T>
bool qlm::Image<frmt, T>::LoadFromFile(const std::string& file_name)
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

	if constexpr (frmt == qlm::ImageFormat::GRAY)
	{
		if (n == 1)
		{
			for (unsigned int i = 0; i < width * height * n; i += n)
			{
				T v = img_data[i];
				data[i].Set(v);
			}
		}
		else if (n == 2)
		{
			for (unsigned int i = 0, idx = 0; i < width * height * n; i += n, idx++)
			{
				T v1 = img_data[i];
				T v2 = img_data[i + 1];
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
			for (unsigned int i = 0; i < width * height * n; i += n)
			{
				T v = img_data[i];
				data[i].Set(v, v, v);
			}
		}
		else if (n == 2)
		{
			for (unsigned int i = 0, idx = 0; i < width * height * n; i += n, idx++)
			{
				T v1 = img_data[i]; // value
				T v2 = img_data[i + 1]; // alpha
				data[idx].Set(v1, v1, v1, v2);
			}
		}
		else if (n == 3)
		{
			for (unsigned int i = 0, idx = 0; i < width * height * n; i += n, idx++)
			{
				T v1 = img_data[i];
				T v2 = img_data[i + 1];
				T v3 = img_data[i + 2];
				data[idx].Set(v1, v2, v3);
			}
		}
		else if (n == 4)
		{
			for (unsigned int i = 0, idx = 0; i < width * height * n; i += n, idx++)
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

template<qlm::ImageFormat frmt, qlm::pixel_t T>
bool qlm::Image<frmt, T>::SaveToFile(const std::string& file_name, bool alpha, int quality)
{
	// I think stb_write supports only U8 !
	
	// check if the data is valid
	if (data == nullptr)
	{
		return false;
	}
	// capture the return value from the stb_image_write functions, which indicates whether the write operation was successful or not.
	int stb_status{ 0 };
	int comp{ 3 };
	T* img_data{ nullptr };
	if constexpr (frmt == qlm::ImageFormat::GRAY)
	{
		comp = 1;
	}
	// check if alpha will be included
	if (alpha)
	{
		comp++;
		//img_data = (T*)data;
		// create new datat that ignores the alpha channel
		img_data = new T[width * height * comp];
		// copy the data 
		if constexpr (frmt == qlm::ImageFormat::GRAY)
		{
			for (unsigned int i = 0, idx = 0; i < width * height * comp;  i += comp, idx++)
			{
				img_data[i] = data[idx].v;
				img_data[i + 1] = data[idx].a;
			}
		}
		else
		{
			for (unsigned int i = 0, idx = 0; i < width * height * comp; i += comp, idx++)
			{
				if constexpr (frmt == ImageFormat::RGB)
				{
					img_data[i] = data[idx].r;
					img_data[i + 1] = data[idx].g;
					img_data[i + 2] = data[idx].b;
					img_data[i + 3] = data[idx].a;
				}
				else if constexpr (frmt == ImageFormat::YCrCb)
				{
					img_data[i] = data[idx].y;
					img_data[i + 1] = data[idx].cr;
					img_data[i + 2] = data[idx].cb;
					img_data[i + 3] = data[idx].a;
				}
				else if constexpr (frmt == ImageFormat::HSV)
				{
					img_data[i] = data[idx].h;
					img_data[i + 1] = data[idx].s;
					img_data[i + 2] = data[idx].v;
					img_data[i + 3] = data[idx].a;
				}
				else
				{
					img_data[i] = data[idx].h;
					img_data[i + 1] = data[idx].l;
					img_data[i + 2] = data[idx].s;
					img_data[i + 3] = data[idx].a;
				}
			}
		}
	}
	else
	{
		// create new datat that ignores the alpha channel
		img_data = new T[width * height * comp];
		// copy the data 
		if constexpr (frmt == qlm::ImageFormat::GRAY)
		{
			for (unsigned int i = 0; i < width * height ; i ++)
			{
				img_data[i] = data[i].v;
			}
		}
		else
		{
			for (unsigned int i = 0, idx = 0; i < width * height * comp; i += comp, idx++)
			{
				if constexpr (frmt == ImageFormat::RGB)
				{
					img_data[i] = data[idx].r;
					img_data[i + 1] = data[idx].g;
					img_data[i + 2] = data[idx].b;
				}
				else if constexpr (frmt == ImageFormat::YCrCb)
				{
					img_data[i] = data[idx].y;
					img_data[i + 1] = data[idx].cr;
					img_data[i + 2] = data[idx].cb;
				}
				else if constexpr (frmt == ImageFormat::HSV)
				{
					img_data[i] = data[idx].h;
					img_data[i + 1] = data[idx].s;
					img_data[i + 2] = data[idx].v;
				}
				else
				{
					img_data[i] = data[idx].h;
					img_data[i + 1] = data[idx].l;
					img_data[i + 2] = data[idx].s;
				}
			}

		}
	}

	std::string ext = file_name.substr(file_name.find_last_of('.') + 1);
	std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

	if (ext == "bmp")
	{
		stb_status = stbi_write_bmp(file_name.c_str(), width, height, comp, reinterpret_cast<void*>(img_data));
	}
	else if (ext == "pgm")
	{
		if (comp == 1)
		{
			// Open the output file for writing
			std::ofstream output_file(file_name, std::ios::binary);
			if (output_file.is_open())
			{
				// Write the PGM header
				output_file << "P5\n" << width << " " << height << "\n" << std::numeric_limits<T>::max() << "\n";

				// Write the image data
				output_file.write(reinterpret_cast<char*>(img_data), width * height * sizeof(T));

				// Close the file
				output_file.close();

				stb_status = 1;
			}
		}
	}
	else if (ext == "png")
	{
		int stride_in_bytes = comp * width * sizeof(T);
		stb_status = stbi_write_png(file_name.c_str(), width, height, comp, reinterpret_cast<void*>(img_data), stride_in_bytes);
	}
	else if (ext == "jpg" || ext == "jpeg")
	{
		stb_status = stbi_write_jpg(file_name.c_str(), width, height, comp, reinterpret_cast<void*>(img_data), quality);
	}
	
	delete[] img_data;

	img_data = nullptr;

	return stb_status ? true : false;
}

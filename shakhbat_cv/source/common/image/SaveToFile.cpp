#include "common/image.h"
#include "stb/stb_image_write.h"
#include <fstream>

namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	bool Image<frmt, T>::SaveToFile(const std::string& file_name, bool alpha, int quality)
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
		if constexpr (frmt == ImageFormat::GRAY)
		{
			comp = 1;
		}
		// check if alpha will be included
		if (alpha)
		{
			comp++;
			//img_data = (T*)data;
			// create new data that ignores the alpha channel
			img_data = new T[width * height * comp];
			// copy the data 
			if constexpr (frmt == ImageFormat::GRAY)
			{
				for (size_t i = 0, idx = 0; i < width * height * comp; i += comp, idx++)
				{
					img_data[i] = data[idx].v;
					img_data[i + 1] = data[idx].a;
				}
			}
			else
			{
				for (size_t i = 0, idx = 0; i < width * height * comp; i += comp, idx++)
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
			// create new data that ignores the alpha channel
			img_data = new T[width * height * comp];
			// copy the data 
			if constexpr (frmt == ImageFormat::GRAY)
			{
				for (size_t i = 0; i < width * height; i++)
				{
					img_data[i] = data[i].v;
				}
			}
			else
			{
				for (size_t i = 0, idx = 0; i < width * height * comp; i += comp, idx++)
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

	template bool Image<ImageFormat::GRAY, uint8_t>::SaveToFile(const std::string&, bool , int);
	template bool Image<ImageFormat::RGB, uint8_t>::SaveToFile(const std::string&, bool, int);
	template bool Image<ImageFormat::GRAY, int16_t>::SaveToFile(const std::string&, bool, int);
	template bool Image<ImageFormat::RGB, int16_t>::SaveToFile(const std::string&, bool, int);
}
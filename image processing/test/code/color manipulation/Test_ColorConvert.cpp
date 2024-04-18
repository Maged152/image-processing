#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_ColorConvert()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Color Manipulation/ColorConvert/";
		const std::string test_name = "Test_ColorConvert";

		// read input image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
		if (!in.LoadFromFile(folder_path + "input.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the input image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}
		// check alpha component
		bool alpha{ true };
		if (in.NumerOfChannels() == 3)
			alpha = false;

		
		// RGB to HSV
		t.start();
		auto out = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t,qlm::ImageFormat::HSV, uint8_t>(in);
		t.end();

		// red to green
		for (int i = 0; i < out.height * out.width; i++)
		{
			auto pix = out.GetPixel(i);
			// extract H value
			int h = (int)pix.h * 2;
			// shift the red color by 120 to make green
			if ((h >= 0 && h <= 30) || h >= 330)
			{
				h += 120;
			}
			// ensure the range 0-360
			if (h > 360)
			{
				h -= 360;
			}
			// to fit in 8-bits
			h /= 2;
			pix.h = h;

			out.SetPixel(i, pix);
		}
		// HSV2RGB
		auto out2 = qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t,qlm::ImageFormat::RGB, uint8_t>(out);

		// write the output and reread it
		if (!out2.SaveToFile("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the output image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		// read output image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> cur;
		if (!cur.LoadFromFile("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the output image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		// read reference image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
		if (!ref.LoadFromFile(folder_path + "result.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the reference image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		res = Test_CompareImages(ref, cur);

		const float normalization = in.width * in.height;
		PrintTestResults(test_name, res, t, normalization, col_handle);

		// delete output image
		if (!std::filesystem::remove("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the output image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		return res;
	}
}
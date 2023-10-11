#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_ConvertBitDepth()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Color Manipulation/FloodFill/";
		const std::string test_name = "Test_ConvertBitDepth";

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
		if (in.NumerOfChannels() == 1)
			alpha = false;

		// U8 to S16
		t.start();
		auto out = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, uint8_t, int16_t>(in);
		t.end();

		// S16 to U8
		auto out_u8 = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, int16_t, uint8_t>(out);
		// read reference image
		
		res = Test_CompareImages(out_u8, in);

		PrintTestResults(test_name, res, t, col_handle);
	
		return res;
	}
}
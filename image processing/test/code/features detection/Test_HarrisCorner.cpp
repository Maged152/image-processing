#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_HarrisCorner()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Features Detection/HarrisCorner/";
		const std::string test_name = "Test_HarrisCorner";

		// read input image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
		if (!in.LoadFromFile(folder_path + "input.png"))
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

		auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

		unsigned int filter_size = 3;
		unsigned int block_size = 3;
		float k = 0.22;
		float threshold = 0.0000001;

		// do the operation
		t.start();
		auto out = qlm::HarrisCorner(gray, block_size, filter_size, k, threshold);
		t.end();

		// draw corners
		qlm::Circle<int> circle = { .radius = 2 };
		qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };

		for (auto& i : out)
		{
			circle.center = i.point;
			in = qlm::DrawCircle(in, circle, green);
		}

		// write the output and reread it
		if (!in.SaveToFile("out.jpg", alpha))
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

		PrintTestResults(test_name, res, t, col_handle);

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
#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_FAST()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Features Detection/FAST/";
		const std::string test_name = "Test_FAST";

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

		auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

		const uint8_t threshold = 70;
		const int arc_len = 9;
		const bool nonmax_suppression = true;

		// do the operation
		t.start();
		auto out = qlm::FAST(gray, arc_len, threshold, nonmax_suppression);
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

		const float normalization = gray.Width() * gray.Height();
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
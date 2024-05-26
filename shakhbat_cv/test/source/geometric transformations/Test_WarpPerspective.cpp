#include "test_cv.h"
#include <filesystem>

namespace test
{
	bool Test_WarpPerspective()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Geometric Transformations/WarpPerspective/";
		const std::string test_name = "Test_WarpPerspective";

		// read input image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
		if (!in.LoadFromFile(folder_path + "input.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the input image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}
		// check alpha component
		bool alpha{ true };
		if (in.NumerOfChannels() == 1)
			alpha = false;
		const int dst_width = 254;
		const int dst_height = 356;

		const qlm::Point<int> src[4] =
		{
			{224, 93}, {430, 137}, {164, 378}, {370, 424}
		};

		const qlm::Point<int> dst[4] =
		{
			{0, 0}, {dst_width, 0}, {0, dst_height}, {dst_width, dst_height}
		};

		qlm::PerspectiveMatrix mat = qlm::GetPerspectiveTransform(src, dst);

		auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
		border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

		// do the operation
		t.start();
		auto out = qlm::WarpPerspective(in, mat, dst_width, dst_height, qlm::InterpolationFlag::BILINEAR, border_mode);
		t.end();

		// write the output and reread it
		if (!out.SaveToFile("out.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the output image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		// read output image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> cur;
		if (!cur.LoadFromFile("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the output image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}
		// read reference image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
		if (!ref.LoadFromFile(folder_path + "result.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the reference image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
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
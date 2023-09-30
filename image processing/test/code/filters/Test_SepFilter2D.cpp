#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_SepFilter2D()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "filters/SepFilter2D/";
		const std::string test_name = "Test_SepFilter2D";

		// read input image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
		if (!in.LoadFromFile(folder_path + "input.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the input image\n";
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

		const int filter_size = 3;

		qlm::SepKernel box{ filter_size, filter_size };
		// set box filter coefficients
		for (int i = 0; i < filter_size; i++)
		{
			box.x_ker.Set(i, 1.0f / filter_size);
			box.y_ker.Set(i, 1.0f / filter_size);
		}

		auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
		border_mode.border_type = qlm::BorderType::BORDER_REFLECT;


		// do the operation
		t.start();
		auto out = qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, box, border_mode);
		t.end();

		// write the output and reread it
		if (!out.SaveToFile("out.jpg"))
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
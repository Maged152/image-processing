#include "test_cv.h"
#include <filesystem>

namespace test
{
	bool Test_Sobel()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Filters/Sobel/";
		const std::string test_name = "Test_Sobel";

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
		if (in.NumerOfChannels() == 3)
			alpha = false;

		// RGB to GRAY
		auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

		size_t filter_size = 3;
		t.start();
		auto out = qlm::Sobel<uint8_t, int16_t>(gray, filter_size);
		t.end();

		// S16 to U8
		auto x = qlm::ConvertSobelDepth(out.sobel_x, filter_size);
		auto y = qlm::ConvertSobelDepth(out.sobel_y, filter_size);

		// write the output and reread it
		if (!x.SaveToFile("outx.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the output x image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		if (!y.SaveToFile("outy.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the output y image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		if (!out.magnitude.SaveToFile("out.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the output image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		
		// read reference image
		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref;
		if (!ref.LoadFromFile(folder_path + "result.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the reference image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> refx;
		if (!ref.LoadFromFile(folder_path + "resultx.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the reference x image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}
		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> refy;
		if (!ref.LoadFromFile(folder_path + "resulty.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the reference y image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		// read output image
		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> cur;
		if (!cur.LoadFromFile("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the output image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> curx;
		if (!cur.LoadFromFile("outx.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the output x image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> cury;
		if (!cur.LoadFromFile("outy.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the output y image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return false;
		}

		res = Test_CompareImages(ref, cur);
		res &= Test_CompareImages(refx, curx);
		res &= Test_CompareImages(refy, cury);

		const float normalization = gray.width * gray.height;
		PrintTestResults(test_name, res, t, normalization, col_handle);

		// delete output image
		if (!std::filesystem::remove("out.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the output image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		if (!std::filesystem::remove("outx.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the output x image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		if (!std::filesystem::remove("outy.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the output y image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		return res;
	}
}

int main()
{
	return !test::Test_Sobel();
}
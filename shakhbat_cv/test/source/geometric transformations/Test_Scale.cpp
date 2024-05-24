#include "test_cv.h"
#include <filesystem>

namespace test
{
	bool Test_Scale()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		std::array<bool, 3> res_bool;
		std::array<float, 3> normalization;
		std::array<qlm::Timer<qlm::usec>, 3> res_time;

		const std::string folder_path = example_folder + "Geometric Transformations/Scale/";
		const std::string test_name = "Test_Scale[NN, BI, CI]";

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

		float scale_x = 2.7f;
		float scale_y = 2.7f;

		res_time[0].start();
		auto out_nn = qlm::Scale(in, qlm::InterpolationFlag::NEAREST_NEIGHBOR, scale_x, scale_y);
		res_time[0].end();

		res_time[1].start();
		auto out_bi = qlm::Scale(in, qlm::InterpolationFlag::BILINEAR, scale_x, scale_y);
		res_time[1].end();

		res_time[2].start();
		auto out_ci = qlm::Scale(in, qlm::InterpolationFlag::BICUBIC, scale_x, scale_y);
		res_time[2].end();


		// write the output and reread it
		if (!out_nn.SaveToFile("out_nn.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the out_nn image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!out_bi.SaveToFile("out_bi.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the out_bi image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!out_ci.SaveToFile("out_ci.jpg", alpha))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to write the out_ci image \n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		// read output image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> cur_nn, cur_bi, cur_ci;

		if (!cur_nn.LoadFromFile("out_nn.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the out_nn image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!cur_bi.LoadFromFile("out_bi.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the out_bi image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!cur_ci.LoadFromFile("out_ci.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the out_ci image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}


		// read reference image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref_nn, ref_bi, ref_ci;

		if (!ref_nn.LoadFromFile(folder_path + "nearest_neighbor.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the nearest_neighbor image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!ref_bi.LoadFromFile(folder_path + "bilinear.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the bilinear image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		if (!ref_ci.LoadFromFile(folder_path + "bicubic.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to read the bi-cubic image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
			return -1;
		}

		res_bool[0] = Test_CompareImages(ref_nn, cur_nn);
		res_bool[1] = Test_CompareImages(ref_bi, cur_bi);
		res_bool[2] = Test_CompareImages(ref_ci, cur_ci);

		normalization[0] = in.width * in.height;
		normalization[1] = in.width * in.height;
		normalization[2] = in.width * in.height;

		PrintTestResults(test_name, res_bool, res_time, normalization, col_handle);

		// delete output image
		if (!std::filesystem::remove("out_nn.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the out_nn image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		if (!std::filesystem::remove("out_bi.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the out_bi image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		if (!std::filesystem::remove("out_ci.jpg"))
		{
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
			std::cout << "Failed to delete the out_ci image\n";
			SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
		}

		bool f_res = res_bool[0] && res_bool[1] && res_bool[2];

		return f_res;
	}
}
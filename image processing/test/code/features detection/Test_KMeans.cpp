#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_KMeans()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Features Detection/KMeans/";
		const std::string test_name = "Test_KMeans";

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

		const unsigned int k = 4;
		const qlm::KMeansInit init = qlm::KMeansInit::KMEANS_PLUS_PLUS;

		// do the operation
		t.start();
		auto clusters = qlm::KMeans(in, k, init);
		t.end();


		// draw clusters
		auto out = qlm::DrawCluster(in, clusters);

		// write the output and reread it
		if (!out.SaveToFile("out.jpg", alpha))
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

		const float normalization = in.Width() * in.Height();
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
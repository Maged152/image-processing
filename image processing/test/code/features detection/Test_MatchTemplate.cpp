#include "../test/test.h"
#include <filesystem>

namespace test
{
	bool Test_MatchTemplate()
	{
		HANDLE col_handle;
		col_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		bool res = false;
		qlm::Timer<qlm::usec> t{};

		const std::string folder_path = example_folder + "Features Detection/MatchTemplate/";
		const std::string test_name = "Test_MatchTemplate";

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

		std::string template_name = "template.jpg";
		// load the template image
		qlm::Image<qlm::ImageFormat::RGB, uint8_t> templ;
		if (!templ.LoadFromFile(folder_path + template_name))
		{
			std::cout << "Failed to read the template image\n";
			return -1;
		}

		qlm::Image<qlm::ImageFormat::GRAY, uint8_t> mask{};
		mask.create(templ.width, templ.height, 1);

		t.start();
		auto out = qlm::MatchTemplate(in, templ, qlm::TemplateMatchFlag::SQDIFF, mask);
		t.end();


		// draw clusters
		auto [min_loc, max_loc] = qlm::MinMaxLoc(out);

		qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };
		qlm::Rectangle rec{ {0, 0}, templ.width, templ.height };

		for (auto& p : min_loc)
		{
			rec.top_left = p;
			in = qlm::DrawRectangle(in, rec, green);
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
#include "test_common.hpp"

TEST(Test_shakhbat_cv, Multiply)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input images
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in1, in2;
	const bool load_in1 = in1.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in1, true);

    in2.Create(in1.width, in1.height, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{255, 255, 255});

    const float scale = 1.0f / 255.0f;

	// perform multiplication
	t.Start();
	auto out = qlm::Multiply<qlm::ImageFormat::RGB, uint8_t, uint8_t, uint8_t>(in1, in2, scale, qlm::OverFlowFlag::WRAP);
	t.End();

	test::PrintTime(t);

	test::CompareImages(out, in1);
}

TEST(Test_shakhbat_cv, Multiply_Overflow)
{
	// all-white x all-white: raw product 65025 -> SATURATE gives 255, WRAP gives 1 (65025 % 256)
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in1(4, 4);
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in2(4, 4);
	for (int i = 0; i < 16; i++)
	{
		in1.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(255));
		in2.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(255));
	}

	auto sat = qlm::Multiply<qlm::ImageFormat::GRAY, uint8_t, uint8_t, uint8_t>(in1, in2, 1.0f, qlm::OverFlowFlag::SATURATE);
	auto wrap = qlm::Multiply<qlm::ImageFormat::GRAY, uint8_t, uint8_t, uint8_t>(in1, in2, 1.0f, qlm::OverFlowFlag::WRAP);

	for (int i = 0; i < 16; i++)
	{
		EXPECT_EQ(sat.GetPixel(i).v, 255);
		EXPECT_EQ(wrap.GetPixel(i).v, 1);
	}
}

TEST(Test_shakhbat_cv, Multiply_Roi)
{
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in1, in2;
	const bool load_in1 = in1.LoadFromFile(folder_path + "input.jpg");
	const bool load_in2 = in2.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in1, true);
	EXPECT_EQ(load_in2, true);

	qlm::Rectangle<int> roi{ qlm::Point<int>{10, 10}, 40, 30 };
	const float scale = 1.0f / 255.0f;

	// allocating + void overloads must agree inside the ROI
	auto out = qlm::Multiply<qlm::ImageFormat::RGB, uint8_t, uint8_t, uint8_t>(in1, in2, scale, qlm::OverFlowFlag::SATURATE, roi);
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_void(in1.width, in1.height);
	qlm::Multiply(in1, in2, out_void, scale, qlm::OverFlowFlag::SATURATE, roi);

	auto [x0, y0, x1, y1] = qlm::ROI(roi, in1.width, in1.height);
	for (int y = y0; y < y1; y++)
	{
		for (int x = x0; x < x1; x++)
		{
			const auto p1 = out.GetPixel(x, y);
			const auto p2 = out_void.GetPixel(x, y);
			EXPECT_EQ(p1.r, p2.r);
			EXPECT_EQ(p1.g, p2.g);
			EXPECT_EQ(p1.b, p2.b);
		}
	}
}

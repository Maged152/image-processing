#include "test_common.hpp"

TEST(Test_shakhbat_cv, Subtract)
{
	qlm::Timer<qlm::usec> t{};

	// synthetic 2x2 images
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in1(2, 2);
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in2(2, 2);

	const uint8_t v1[4] = { 200, 10, 250, 128 };
	const uint8_t v2[4] = { 100, 20, 0,   128 };

	for (int i = 0; i < 4; i++)
	{
		in1.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(v1[i]));
		in2.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(v2[i]));
	}

	// in1 - in2 (saturating int16 output)
	t.Start();
	auto out = qlm::Subtract<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in1, in2);
	t.End();
	test::PrintTime(t);

	EXPECT_EQ(out.GetPixel(0).v, 100);
	EXPECT_EQ(out.GetPixel(1).v, -10);
	EXPECT_EQ(out.GetPixel(2).v, 250);
	EXPECT_EQ(out.GetPixel(3).v, 0);

	// identity: in - in == 0 everywhere
	auto zero = qlm::Subtract<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in1, in1);
	for (int i = 0; i < 4; i++)
	{
		EXPECT_EQ(zero.GetPixel(i).v, 0);
	}
}

TEST(Test_shakhbat_cv, Subtract_Roi)
{
	qlm::Timer<qlm::usec> t{};

	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> rgb;
	const bool load_in = rgb.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(rgb);

	// in - in == 0 inside the ROI; both overloads must agree
	qlm::Rectangle<int> roi{ qlm::Point<int>{10, 10}, 40, 30 };

	t.Start();
	auto out = qlm::Subtract<qlm::ImageFormat::GRAY, uint8_t, int16_t>(gray, gray, roi);
	qlm::Image<qlm::ImageFormat::GRAY, int16_t> out_void(gray.width, gray.height);
	qlm::Subtract(gray, gray, out_void, roi);
	t.End();
	test::PrintTime(t);

	for (int y = 0; y < gray.height; y++)
	{
		for (int x = 0; x < gray.width; x++)
		{
			EXPECT_EQ(out.GetPixel(x, y).v, out_void.GetPixel(x, y).v);
		}
	}
}
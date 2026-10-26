#include "test_common.hpp"

TEST(Test_shakhbat_cv, Max)
{
	qlm::Timer<qlm::usec> t{};

	// synthetic 3x2 gray image with known values
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(3, 2);
	const uint8_t vals[6] = { 200, 10, 250, 128, 0, 77 };

	for (int i = 0; i < 6; i++)
	{
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(vals[i]));
	}

	t.Start();
	auto result = qlm::Max<qlm::ImageFormat::GRAY, uint8_t>(in);
	t.End();
	test::PrintTime(t);

	// max pixel value over the whole image (alpha is the same in all pixels)
	EXPECT_EQ(result.v, 250);
	EXPECT_EQ(result.a, 255);

	// consistency between the two overloads
	qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> out_void;
	qlm::Max(in, out_void);

	EXPECT_EQ(out_void.v, result.v);
	EXPECT_EQ(out_void.a, result.a);
}

TEST(Test_shakhbat_cv, Max_Roi)
{
	qlm::Timer<qlm::usec> t{};

	// synthetic 4x3 gray image with known values
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(4, 3);
	const uint8_t vals[12] = { 50,  200, 30,  120,
							   250, 5,   150, 90,
							   10,  180, 220, 60 };

	for (int i = 0; i < 12; i++)
	{
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(vals[i]));
	}

	// ROI: x = [1, 3), y = [0, 3) -> the max inside {200, 30, 5, 150, 180, 220} is 220
	qlm::Rectangle<int> roi{ qlm::Point<int>{1, 0}, 2, 3 };

	t.Start();
	auto result = qlm::Max<qlm::ImageFormat::GRAY, uint8_t>(in, roi);
	t.End();
	test::PrintTime(t);

	EXPECT_EQ(result.v, 220);
	EXPECT_EQ(result.a, 255);

	// ROI covering only the first row -> max = 200
	qlm::Rectangle<int> roi_first_row{ qlm::Point<int>{0, 0}, 4, 1 };
	auto result_first_row = qlm::Max<qlm::ImageFormat::GRAY, uint8_t>(in, roi_first_row);
	EXPECT_EQ(result_first_row.v, 200);

	// out-of-range ROI is clamped to the image -> same result as the whole image
	qlm::Rectangle<int> roi_big{ qlm::Point<int>{0, 0}, 100, 100 };
	auto result_big = qlm::Max<qlm::ImageFormat::GRAY, uint8_t>(in, roi_big);
	EXPECT_EQ(result_big.v, 250);
}


TEST(Test_shakhbat_cv, Max_Float)
{
	qlm::Timer<qlm::usec> t{};

	// synthetic 2x2 gray float image with known values
	qlm::Image<qlm::ImageFormat::GRAY, float> in(2, 2);
	const float vals[4] = { 0.25f, -1.5f, 9.0f, 3.25f };

	for (int i = 0; i < 4; i++)
	{
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, float>(vals[i]));
	}

	t.Start();
	auto result = qlm::Max<qlm::ImageFormat::GRAY, float>(in);
	t.End();
	test::PrintTime(t);

	// max pixel value over the whole image (alpha is the same in all pixels)
	EXPECT_EQ(result.v, 9.0f);
	EXPECT_EQ(result.a, std::numeric_limits<float>::max());

	// negative ROI is clamped -> same result as the whole image
	qlm::Rectangle<int> roi{ qlm::Point<int>{-5, -5}, 2, 2 };
	auto result_roi = qlm::Max<qlm::ImageFormat::GRAY, float>(in, roi);
	EXPECT_EQ(result_roi.v, 9.0f);
}

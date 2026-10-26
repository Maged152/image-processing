#include "test_common.hpp"

TEST(Test_shakhbat_cv, Min)
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
	auto result = qlm::Min<qlm::ImageFormat::GRAY, uint8_t>(in);
	t.End();
	test::PrintTime(t);

	// min pixel value over the whole image (alpha is the same in all pixels)
	EXPECT_EQ(result.v, 0);
	EXPECT_EQ(result.a, 255);

	// consistency between the two overloads
	qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t> out_void;
	qlm::Min(in, out_void);

	EXPECT_EQ(out_void.v, result.v);
	EXPECT_EQ(out_void.a, result.a);
}

TEST(Test_shakhbat_cv, Min_Roi)
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

	// ROI: x = [1, 3), y = [0, 3) -> the min inside {200, 30, 5, 150, 180, 220} is 5
	qlm::Rectangle<int> roi{ qlm::Point<int>{1, 0}, 2, 3 };

	t.Start();
	auto result = qlm::Min<qlm::ImageFormat::GRAY, uint8_t>(in, roi);
	t.End();
	test::PrintTime(t);

	EXPECT_EQ(result.v, 5);
	EXPECT_EQ(result.a, 255);

	// ROI covering only the last row -> min = 10
	qlm::Rectangle<int> roi_last_row{ qlm::Point<int>{0, 2}, 4, 1 };
	auto result_last_row = qlm::Min<qlm::ImageFormat::GRAY, uint8_t>(in, roi_last_row);
	EXPECT_EQ(result_last_row.v, 10);

	// out-of-range ROI is clamped to the image -> same result as the whole image (min of the image = 5)
	qlm::Rectangle<int> roi_big{ qlm::Point<int>{0, 0}, 100, 100 };
	auto result_big = qlm::Min<qlm::ImageFormat::GRAY, uint8_t>(in, roi_big);
	EXPECT_EQ(result_big.v, 5);
}

TEST(Test_shakhbat_cv, Min_Float)
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
	auto result = qlm::Min<qlm::ImageFormat::GRAY, float>(in);
	t.End();
	test::PrintTime(t);

	// min pixel value over the whole image (alpha is the same in all pixels)
	EXPECT_EQ(result.v, -1.5f);
	EXPECT_EQ(result.a, std::numeric_limits<float>::max());

	// negative ROI is clamped -> same result as the whole image
	qlm::Rectangle<int> roi{ qlm::Point<int>{-5, -5}, 2, 2 };
	auto result_roi = qlm::Min<qlm::ImageFormat::GRAY, float>(in, roi);
	EXPECT_EQ(result_roi.v, -1.5f);
}

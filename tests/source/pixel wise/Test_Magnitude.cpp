#include "test_common.hpp"

TEST(Test_shakhbat_cv, Magnitude_Scharr)
{
	qlm::Timer<qlm::usec> t_l1{}, t_l2{};
	const std::string folder_path = test::example_folder + "Pixel-Wise/Magnitude/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

	auto gx = qlm::ScharrX<uint8_t, int16_t>(gray);
	auto gy = qlm::ScharrY<uint8_t, int16_t>(gray);

	t_l1.Start();
	auto out_l1 = qlm::Magnitude(gx, gy, false);
	t_l1.End();

	t_l2.Start();
	auto out_l2 = qlm::Magnitude(gx, gy, true);
	t_l2.End();

	test::PrintTime(t_l1);
	test::PrintTime(t_l2);

	// L2 result: cross-check every pixel against an independent computation
	for (int i = 0; i < gx.Width() * gx.Height(); i++)
	{
		const double x = static_cast<double>(gx.GetPixel(i).v);
		const double y = static_cast<double>(gy.GetPixel(i).v);
		const uint16_t expected = static_cast<uint16_t>(std::sqrt(x * x + y * y));
		EXPECT_EQ(out_l2.GetPixel(i).v, expected);
	}

	// L1 result: |gx| + |gy| (clamped to uint16_t)
	for (int i = 0; i < gx.Width() * gx.Height(); i++)
	{
		const uint16_t expected = static_cast<uint16_t>(
			std::min(std::abs(static_cast<int>(gx.GetPixel(i).v)) + std::abs(static_cast<int>(gy.GetPixel(i).v)),
			         static_cast<int>(std::numeric_limits<uint16_t>::max())));
		EXPECT_EQ(out_l1.GetPixel(i).v, expected);
	}

	// convert bit depth to u8 for viewing
	auto view_l2 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint16_t, uint8_t>(out_l2);
	auto view_l1 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint16_t, uint8_t>(out_l1);

	// reread output images
	view_l2 = test::ReReadImage(view_l2);
	view_l1 = test::ReReadImage(view_l1);

	// read reference images
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref_l2;
	const bool load_ref_l2 = ref_l2.Read(folder_path + "result_l2.jpg");
	EXPECT_EQ(load_ref_l2, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref_l1;
	const bool load_ref_l1 = ref_l1.Read(folder_path + "result_l1.jpg");
	EXPECT_EQ(load_ref_l1, true);

	test::CompareImages(view_l2, ref_l2);
	test::CompareImages(view_l1, ref_l1);
}

TEST(Test_shakhbat_cv, Magnitude_Identity)
{
	qlm::Timer<qlm::usec> t_l1{}, t_l2{};
	const std::string folder_path = test::example_folder + "Pixel-Wise/Magnitude/";

	const int width = 640;
	const int height = 480;

	// read input image
	qlm::Image<qlm::ImageFormat::GRAY, int16_t> in_s16(width, height);
	qlm::Image<qlm::ImageFormat::GRAY, uint16_t> in_u16(width, height);

	// randomly initialize the images
	for (int i = 0; i < in_s16.Width() * in_s16.Height(); i++)
	{
		int16_t val = static_cast<int16_t>(std::rand() % 32767);
		in_s16.SetPixel(i, val);
		in_u16.SetPixel(i, val);
	}

	// zero image of the same size
	qlm::Image<qlm::ImageFormat::GRAY, int16_t> zero(width, height);

	// |in - 0| == in for both L1 and L2
	t_l1.Start();
	auto out_l1 = qlm::Magnitude(in_s16, zero, false);
	t_l1.End();

	t_l2.Start();
	auto out_l2 = qlm::Magnitude(in_s16, zero, true);
	t_l2.End();

	test::PrintTime(t_l1);
	test::PrintTime(t_l2);

	test::CompareImages(out_l1, in_u16);
	test::CompareImages(out_l2, in_u16);
}

// The in-place overload (both L1 and L2, plus its roi parameter) is never
// exercised by the allocating-only tests above.
TEST(Test_shakhbat_cv, Magnitude_InPlace)
{
	const int width = 320;
	const int height = 240;

	// random gradients
	qlm::Image<qlm::ImageFormat::GRAY, int16_t> gx(width, height), gy(width, height);
	for (int i = 0; i < width * height; i++)
	{
		gx.SetPixel(i, static_cast<int16_t>(std::rand() % 2048 - 1024));
		gy.SetPixel(i, static_cast<int16_t>(std::rand() % 2048 - 1024));
	}

	// allocate the reference results with the allocating overload
	auto out_l1 = qlm::Magnitude(gx, gy, false);
	auto out_l2 = qlm::Magnitude(gx, gy, true);

	// in-place overloads must produce identical images
	qlm::Image<qlm::ImageFormat::GRAY, uint16_t> void_l1(width, height), void_l2(width, height);
	qlm::Magnitude(gx, gy, void_l1, false);
	qlm::Magnitude(gx, gy, void_l2, true);

	test::CompareImages(void_l1, out_l1);
	test::CompareImages(void_l2, out_l2);

	// roi: only pixels inside the roi are written, the rest keeps its old value
	const uint16_t seed = 123;
	qlm::Rectangle<int> roi{ qlm::Point<int>{50, 40}, 80, 60 };
	qlm::Image<qlm::ImageFormat::GRAY, uint16_t> roi_out(width, height);
	for (int i = 0; i < width * height; i++)
	{
		roi_out.SetPixel(i, seed);
	}
	qlm::Magnitude(gx, gy, roi_out, true, roi);

	auto [x0, y0, x1, y1] = qlm::ROI(roi, width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (x >= x0 && x < x1 && y >= y0 && y < y1)
			{
				EXPECT_EQ(roi_out.GetPixel(x, y).v, out_l2.GetPixel(x, y).v);
			}
			else
			{
				EXPECT_EQ(roi_out.GetPixel(x, y).v, seed);
			}
		}
	}
}

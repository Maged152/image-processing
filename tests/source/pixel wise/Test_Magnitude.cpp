#include "test_common.hpp"

TEST(Test_shakhbat_cv, Magnitude_Scharr)
{
	qlm::Timer<qlm::usec> t_l1{}, t_l2{};
	const std::string folder_path = test::example_folder + "Pixel-Wise/Magnitude/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

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
	for (int i = 0; i < gx.width * gx.height; i++)
	{
		const double x = static_cast<double>(gx.GetPixel(i).v);
		const double y = static_cast<double>(gy.GetPixel(i).v);
		const uint16_t expected = static_cast<uint16_t>(std::sqrt(x * x + y * y));
		EXPECT_EQ(out_l2.GetPixel(i).v, expected);
	}

	// L1 result: |gx| + |gy| (clamped to uint16_t)
	for (int i = 0; i < gx.width * gx.height; i++)
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
	const bool load_ref_l2 = ref_l2.LoadFromFile(folder_path + "result_l2.jpg");
	EXPECT_EQ(load_ref_l2, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref_l1;
	const bool load_ref_l1 = ref_l1.LoadFromFile(folder_path + "result_l1.jpg");
	EXPECT_EQ(load_ref_l1, true);

	test::CompareImages(view_l2, ref_l2);
	test::CompareImages(view_l1, ref_l1);
}

TEST(Test_shakhbat_cv, Magnitude_Identity)
{
	qlm::Timer<qlm::usec> t_l1{}, t_l2{};
	const std::string folder_path = test::example_folder + "Pixel-Wise/Magnitude/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

	// zero image of the same size
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> zero(gray.width, gray.height);

	// |in - 0| == in for both L1 and L2
	t_l1.Start();
	auto out_l1 = qlm::Magnitude(gray, zero, false);
	t_l1.End();

	t_l2.Start();
	auto out_l2 = qlm::Magnitude(gray, zero, true);
	t_l2.End();

	test::PrintTime(t_l1);
	test::PrintTime(t_l2);

	test::CompareImages(out_l1, gray);
	test::CompareImages(out_l2, gray);
}

#include "test_common.hpp"

TEST(Test_shakhbat_cv, FAST)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/FAST/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

	const uint8_t threshold = 70;
	const int arc_len = 9;
	const bool nonmax_suppression = true;

	// do the operation
	t.Start();
	auto out = qlm::FAST(gray, arc_len, threshold, nonmax_suppression);
	t.End();

	// draw corners
	qlm::Circle<int> circle = { .radius = 2 };
	qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };

	for (auto& i : out)
	{
		circle.center = i.point;
		in = qlm::DrawCircle(in, circle, green);
	}

	test::PrintTime(t);

	// reread output image
	in = test::ReReadImage(in);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(in, ref);
}

TEST(Test_shakhbat_cv, FAST_Variants)
{
	const std::string folder_path = test::example_folder + "Features Detection/FAST/";

	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	ASSERT_TRUE(in.Read(folder_path + "input.jpg"));
	const auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

	const uint8_t threshold = 70;

	// arc 12 is stricter than arc 9: subset of corners
	const auto corners9 = qlm::FAST(gray, 9, threshold, true);
	const auto corners12 = qlm::FAST(gray, 12, threshold, true);
	EXPECT_LE(corners12.size(), corners9.size());

	// without NMS we get at least as many corners as with NMS
	const auto corners_no_nms = qlm::FAST(gray, 9, threshold, false);
	EXPECT_GE(corners_no_nms.size(), corners9.size());

	// all key-points respect the 3-pixel border margin
	for (const auto& kp : corners9)
	{
		EXPECT_GE(kp.point.x, 3);
		EXPECT_GE(kp.point.y, 3);
		EXPECT_LT(kp.point.x, gray.Width() - 3);
		EXPECT_LT(kp.point.y, gray.Height() - 3);
	}
}

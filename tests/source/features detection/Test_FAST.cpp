#include "test_common.hpp"

TEST(Test_shakhbat_cv, FAST)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/FAST/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

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
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(in, ref);
}
#include "test_common.h"

TEST(Test_shakhbat_cv, HarrisCorner)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/HarrisCorner/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.png");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

	size_t filter_size = 3;
	size_t block_size = 3;
	float k = 0.22;
	float threshold = 0.0000001;

	// do the operation
	t.start();
	auto out = qlm::HarrisCorner(gray, block_size, filter_size, k, threshold);
	t.end();

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

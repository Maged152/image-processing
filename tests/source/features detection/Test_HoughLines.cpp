#include "test_common.hpp"

TEST(Test_shakhbat_cv, HoughLines)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/HoughLines/";

	// read input image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.png");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	// do the operation
	t.start();
	std::vector<qlm::LinePolar> lines = qlm::HoughLines(in, 1, 3 * 3.14f / 180, 110);
	t.end();

	// out image to draw on
	auto draw = qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(in);

	for (auto& line : lines)
	{
		draw = qlm::DrawLine(draw, line, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{ 0, 255, 0 });
	}

	test::PrintTime(t);

	// reread output image
	draw = test::ReReadImage(draw);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(draw, ref);
}
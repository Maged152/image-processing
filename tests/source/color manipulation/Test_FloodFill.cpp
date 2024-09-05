#include "test_common.h"

TEST(Test_shakhbat_cv, FloodFill)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/FloodFill/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	// do the operation
	qlm::Point seed{ 300 ,300 };
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{ 0,0,0 };
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> threshold{ 20,20,20 };
	
	t.start();
	auto out = qlm::FloodFill(in, seed, color, threshold);
	t.end();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}
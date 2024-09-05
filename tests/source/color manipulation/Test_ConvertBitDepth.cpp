#include "test_common.h"

TEST(Test_shakhbat_cv, ConvertBitDepth)
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
	// U8 to S16
	t.start();
	auto out = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, uint8_t, int16_t>(in);
	t.end();

	test::PrintTime(t);

	// S16 to U8
	auto out_u8 = qlm::ConvertBitDepth<qlm::ImageFormat::RGB, int16_t, uint8_t>(out);

	test::CompareImages(out_u8, in);
}
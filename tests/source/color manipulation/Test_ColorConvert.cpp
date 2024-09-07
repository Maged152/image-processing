#include "test_common.hpp"

TEST(Test_shakhbat_cv, ColorConvert)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/ColorConvert/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;
	
	// do the operation
	// RGB to HSV
	t.start();
	auto out = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t,qlm::ImageFormat::HSV, uint8_t>(in);
	t.end();

	// red to green
	for (int i = 0; i < out.height * out.width; i++)
	{
		auto pix = out.GetPixel(i);
		// extract H value
		int h = (int)pix.h * 2;
		// shift the red color by 120 to make green
		if ((h >= 0 && h <= 30) || h >= 330)
		{
			h += 120;
		}
		// ensure the range 0-360
		if (h > 360)
		{
			h -= 360;
		}
		// to fit in 8-bits
		h /= 2;
		pix.h = h;

		out.SetPixel(i, pix);
	}
	// HSV2RGB
	auto out2 = qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t,qlm::ImageFormat::RGB, uint8_t>(out);

	test::PrintTime(t);

	// reread output image
	out2 = test::ReReadImage(out2);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out2, ref);
}
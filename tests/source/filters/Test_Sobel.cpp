#include "test_common.hpp"

TEST(Test_shakhbat_cv, Sobel)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/Sobel/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

	// do the operation
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;

	// RGB to GRAY
	auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

	size_t filter_size = 3;
	t.start();
	auto out = qlm::Sobel<uint8_t, int16_t>(gray, filter_size);
	t.end();

	// S16 to U8
	auto x = qlm::ConvertSobelDepth(out.sobel_x, filter_size);
	auto y = qlm::ConvertSobelDepth(out.sobel_y, filter_size);

	test::PrintTime(t);

	// reread output image
	x = test::ReReadImage(x);
	y = test::ReReadImage(y);
	auto m = test::ReReadImage(out.magnitude);

	// read reference image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref_x;
	const bool load_ref_x = ref_x.LoadFromFile(folder_path + "resultx.jpg");
	EXPECT_EQ(load_ref_x, true);

	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref_y;
	const bool load_ref_y = ref_y.LoadFromFile(folder_path + "resulty.jpg");
	EXPECT_EQ(load_ref_y, true);

	test::CompareImages(x, ref_x);
	test::CompareImages(y, ref_y);
	test::CompareImages(m, ref);
}
#include "test_common.hpp"

TEST(Test_shakhbat_cv, EqualizeHist)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Pixel-Wise/EqualizeHist/";

	// read input image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.png");
	EXPECT_EQ(load_in, true);

	// do the operation
	t.Start();
	auto out = qlm::EqualizeHist(in);
	t.End();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result_histogram.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}

TEST(Test_shakhbat_cv, EqualizeHist_ConstantImage)
{
	// all-zero image: denominator (N - cum[0]) is zero -> returned unchanged, no crash
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(8, 8);
	for (int i = 0; i < 64; i++)
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(0));

	auto out = qlm::EqualizeHist(in);

	test::CompareImages(out, in);
}
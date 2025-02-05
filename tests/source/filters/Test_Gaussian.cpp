#include "test_common.hpp"

TEST(Test_shakhbat_cv, Gaussian)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/Gaussian/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

	// do the operation
	int filter_size = 3;
	float sigma = 1.0f;
	t.Start();
	auto out = qlm::Gaussian<qlm::ImageFormat::RGB, uint8_t>(in, filter_size, sigma);
	t.End();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);		
}
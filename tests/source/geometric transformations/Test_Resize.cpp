#include "test_common.hpp"

TEST(Test_shakhbat_cv, Resize)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Resize/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	int width = 960u;
	int height = 540u;

	// do the operation
	t.Start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::Resize(in, width, height);
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
TEST(Test_shakhbat_cv, Resize_Identity)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Resize/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// resizing to the same dimensions must reproduce the input
	t.Start();
	auto out = qlm::Resize(in, in.width, in.height, qlm::InterpolationFlag::BILINEAR);
	t.End();

	test::PrintTime(t);

	// the output must be identical to the input
	test::CompareImages(out, in);
}

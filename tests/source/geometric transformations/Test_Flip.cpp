#include "test_common.hpp"

TEST(Test_shakhbat_cv, Flip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Flip/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	// do the operation
	t.Start();
	auto out = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_AXIS);
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
TEST(Test_shakhbat_cv, Flip_RoundTrip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Flip/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// two flips in each direction must restore the original image
	t.Start();
	auto out_v = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_AXIS);
	out_v = qlm::Flip(out_v, qlm::FlipFlag::FLIP_VERTICAL_AXIS);

	auto out_h = qlm::Flip(in, qlm::FlipFlag::FLIP_HORIZONTAL_AXIS);
	out_h = qlm::Flip(out_h, qlm::FlipFlag::FLIP_HORIZONTAL_AXIS);

	auto out_bh = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_HORIZONTAL_AXIS);
	out_bh = qlm::Flip(out_bh, qlm::FlipFlag::FLIP_VERTICAL_HORIZONTAL_AXIS);
	t.End();

	test::PrintTime(t);

	// the outputs must be identical to the input
	test::CompareImages(out_v, in);
	test::CompareImages(out_h, in);
	test::CompareImages(out_bh, in);
}

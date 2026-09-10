#include "test_common.hpp"

TEST(Test_shakhbat_cv, Rotate)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Rotate/";

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
	auto out = qlm::Rotate(in, qlm::RotateFlag::ROTATE_90_CLOCKWISE);
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

TEST(Test_shakhbat_cv, Rotate_RoundTrip_180)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Rotate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// two 180 rotations must restore the original image
	t.Start();
	auto out = qlm::Rotate(in, qlm::RotateFlag::ROTATE_180);
	out = qlm::Rotate(out, qlm::RotateFlag::ROTATE_180);
	t.End();

	test::PrintTime(t);

	// dimensions must match the input
	EXPECT_EQ(out.width, in.width);
	EXPECT_EQ(out.height, in.height);

	// the output must be identical to the input
	test::CompareImages(out, in);
}

TEST(Test_shakhbat_cv, Rotate_RoundTrip_90_270)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Rotate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// 90 CW followed by 90 CCW must restore the original image
	t.Start();
	auto out_90 = qlm::Rotate(in, qlm::RotateFlag::ROTATE_90_CLOCKWISE);
	auto out = qlm::Rotate(out_90, qlm::RotateFlag::ROTATE_90_COUNTERCLOCKWISE);
	t.End();

	test::PrintTime(t);

	// the 90 rotation swaps the dimensions (non-square input)
	EXPECT_EQ(out_90.width, in.height);
	EXPECT_EQ(out_90.height, in.width);

	// back to the input dimensions
	EXPECT_EQ(out.width, in.width);
	EXPECT_EQ(out.height, in.height);

	// the output must be identical to the input
	test::CompareImages(out, in);
}
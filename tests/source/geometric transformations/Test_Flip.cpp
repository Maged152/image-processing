#include "test_common.hpp"

TEST(Test_shakhbat_cv, Flip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Flip/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
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
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}
TEST(Test_shakhbat_cv, Flip_RoundTrip)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Flip/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
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

// Odd dimensions: with FLIP_VERTICAL_HORIZONTAL_AXIS an odd-height image's
// middle row must still be flipped horizontally (previously left as default
// pixels), and every flip must not disturb the untouched middle column/row.
TEST(Test_shakhbat_cv, Flip_OddSizes)
{
	// 7x5 GRAY image with distinct values
	const int width = 7;
	const int height = 5;
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(width, height);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			in.SetPixel(x, y, static_cast<uint8_t>(y * width + x + 1));
		}
	}

	// vertical axis flip: compare against a manual mirror
	auto vert = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_AXIS);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			EXPECT_EQ(vert.GetPixel(x, y).v, in.GetPixel(width - 1 - x, y).v);
		}
	}

	// horizontal axis flip
	auto horz = qlm::Flip(in, qlm::FlipFlag::FLIP_HORIZONTAL_AXIS);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			EXPECT_EQ(horz.GetPixel(x, y).v, in.GetPixel(x, height - 1 - y).v);
		}
	}

	// both axes: known top-left/bottom-right swap plus a flipped middle row
	auto both = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_HORIZONTAL_AXIS);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			EXPECT_EQ(both.GetPixel(x, y).v, in.GetPixel(width - 1 - x, height - 1 - y).v);
		}
	}
}

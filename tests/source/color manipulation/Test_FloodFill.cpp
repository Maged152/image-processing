#include "test_common.hpp"

TEST(Test_shakhbat_cv, FloodFill)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Color Manipulation/FloodFill/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	// do the operation
	qlm::Point seed{ 300 ,300 };
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{ 0,0,0 };
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> threshold{ 20,20,20 };
	
	t.Start();
	auto out = qlm::FloodFill(in, seed, color, threshold);
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

TEST(Test_shakhbat_cv, FloodFill_NoOp)
{
	const std::string folder_path = test::example_folder + "Color Manipulation/FloodFill/";

	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	qlm::Point<int> seed{ 300, 300 };
	const auto seed_color = in.GetPixel(seed.x, seed.y);

	// fill color equals the seed pixel: region matches immediately -> unchanged copy
	auto out = qlm::FloodFill(in, seed, seed_color, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{20, 20, 20});
	test::CompareImages(out, in);

	// out-of-bounds seed: unchanged copy, no crash
	qlm::Point<int> bad_seed{ -1, -1 };
	auto out2 = qlm::FloodFill(in, bad_seed, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{0, 0, 0});
	test::CompareImages(out2, in);
}

TEST(Test_shakhbat_cv, FloodFill_Synthetic)
{
	// 5x5 GRAY canvas with a 3x2 block of 100s; fill it with 0
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(5, 5);
	for (int i = 0; i < 25; i++)
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(50));

	// block rows 1-2, cols 1-3
	for (int y = 1; y <= 2; y++)
		for (int x = 1; x <= 3; x++)
			in.SetPixel(x, y, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(100));

	auto out = qlm::FloodFill(in, qlm::Point<int>{1, 1},
		qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(0),
		qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(10));

	// block filled with 0, background untouched at 50
	for (int y = 1; y <= 2; y++)
		for (int x = 1; x <= 3; x++)
			EXPECT_EQ(out.GetPixel(x, y).v, 0);

	for (int y = 0; y < 5; y++)
		for (int x = 0; x < 5; x++)
			if (!(y >= 1 && y <= 2 && x >= 1 && x <= 3))
				EXPECT_EQ(out.GetPixel(x, y).v, 50);
}
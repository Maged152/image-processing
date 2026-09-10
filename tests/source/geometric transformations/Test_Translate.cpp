#include "test_common.hpp"

TEST(Test_shakhbat_cv, Translate)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Translate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;


	qlm::Point<int> displacement{ 50 ,50 };
	// do the operation
	t.Start();
	auto out = qlm::Translate(in, displacement);
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
TEST(Test_shakhbat_cv, Translate_Identity)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Translate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// a zero displacement must reproduce the input
	const qlm::Point<int> displacement{ 0, 0 };

	t.Start();
	auto out = qlm::Translate(in, displacement);
	t.End();

	test::PrintTime(t);

	// the output must be identical to the input
	test::CompareImages(out, in);
}

TEST(Test_shakhbat_cv, Translate_SubPixel)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Translate/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	const qlm::Point<int> int_disp{ 12, 7 };
	const qlm::Point<float> float_disp{ 12.0f, 7.0f };

	// an integer-valued float displacement must match the int path exactly
	t.Start();
	auto out_f = qlm::Translate(in, float_disp);
	auto out_i = qlm::Translate(in, int_disp);
	t.End();

	test::PrintTime(t);

	EXPECT_EQ(out_f.width, in.width);
	EXPECT_EQ(out_f.height, in.height);
	test::CompareImages(out_f, out_i);

	// true sub-pixel displacement: check one interior pixel against an
	// independently computed bilinear blend of its four source neighbors
	const float dx = 12.5f, dy = 7.25f;
	auto out_sub = qlm::Translate(in, qlm::Point<float>{ dx, dy });

	const int sx = 20, sy = 15;
	const float fx = sx - dx;   // 7.5
	const float fy = sy - dy;   // 7.75
	const int x0 = (int)fx;
	const int y0 = (int)fy;
	const float xoff = fx - x0;
	const float yoff = fy - y0;

	const auto p00 = in.GetPixel(x0, y0);
	const auto p10 = in.GetPixel(x0 + 1, y0);
	const auto p01 = in.GetPixel(x0, y0 + 1);
	const auto p11 = in.GetPixel(x0 + 1, y0 + 1);

	auto lerp = [](float a, float b, float t)
	{
		return a * (1.0f - t) + b * t;
	};

	const float exp_r = lerp(lerp((float)p00.r, (float)p10.r, xoff), lerp((float)p01.r, (float)p11.r, xoff), yoff);
	const float exp_g = lerp(lerp((float)p00.g, (float)p10.g, xoff), lerp((float)p01.g, (float)p11.g, xoff), yoff);
	const float exp_b = lerp(lerp((float)p00.b, (float)p10.b, xoff), lerp((float)p01.b, (float)p11.b, xoff), yoff);

	const auto got = out_sub.GetPixel(sx, sy);

	// the implementation rounds through intermediate uint8 pixels, so allow
	// a small tolerance around the exact blend
	EXPECT_LE(std::abs((int)got.r - (int)exp_r), 3);
	EXPECT_LE(std::abs((int)got.g - (int)exp_g), 3);
	EXPECT_LE(std::abs((int)got.b - (int)exp_b), 3);

	// pixels whose source lies outside the input must be the background pixel
	const auto bg = out_sub.GetPixel(5, sy);   // 5 - 12.5 < 0
	EXPECT_EQ(bg.r, 0);
	EXPECT_EQ(bg.g, 0);
	EXPECT_EQ(bg.b, 0);
}

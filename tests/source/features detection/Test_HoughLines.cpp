#include "test_common.hpp"
#include <numbers>

TEST(Test_shakhbat_cv, HoughLines)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Features Detection/HoughLines/";

	// read input image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.png");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 1)
		alpha = false;

	// do the operation
	t.Start();
	std::vector<qlm::LinePolar> lines = qlm::HoughLines(in, 1, 3 * std::numbers::pi_v<float> / 180.0f, 110);
	t.End();

	// out image to draw on
	auto draw = qlm::ColorConvert<qlm::ImageFormat::RGB>(in);

	for (auto& line : lines)
	{
		draw = qlm::DrawLine(draw, line, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{ 0, 255, 0 });
	}

	test::PrintTime(t);

	// reread output image
	draw = test::ReReadImage(draw);
	
	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(draw, ref);
}
TEST(Test_shakhbat_cv, HoughLines_Synthetic)
{
	// 100x100 canvas with a single horizontal white line at y=40
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(100, 100);
	for (int x = 0; x < 100; x++)
		 in.SetPixel(x, 40, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(255));
		// a horizontal line y=40 has normal theta=pi/2, rho=40
		// (x*cos(pi/2)+40*sin(pi/2) = 40 for every x)
		const float theta_step = 3 * std::numbers::pi_v<float> / 180.0f;
		const auto lines = qlm::HoughLines(in, 1.0f, theta_step, 50);
		ASSERT_FALSE(lines.empty());
		bool found = false;
		for (const auto &l : lines)
		{
			if (std::abs(l.radius - 40.0f) <= 1.0f && std::abs(l.angle - std::numbers::pi / 2) <= theta_step)
				 found = true;
		}
		EXPECT_TRUE(found);
	}


#include "test_common.hpp"

TEST(Test_shakhbat_cv, Multiply)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input images
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in1, in2;
	const bool load_in1 = in1.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in1, true);

    in2.Create(in1.width, in1.height, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{255, 255, 255});

    const float scale = 1.0f / 255.0f;

	// perform multiplication
	t.Start();
	auto out = qlm::Multiply<qlm::ImageFormat::RGB, uint8_t, uint8_t, uint8_t>(in1, in2, scale, qlm::OverFlowFlag::WRAP);
	t.End();

	test::PrintTime(t);

	test::CompareImages(out, in1);
}

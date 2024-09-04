#include "test_common.h"

TEST(Test_shakhbat_cv, SepFilter2D)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "filters/SepFilter2D/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

	// do the operation
	const int filter_size = 3;

	qlm::SepKernel box{ filter_size, filter_size };
	// set box filter coefficients
	for (int i = 0; i < filter_size; i++)
	{
		box.x_ker.Set(i, 1.0f / filter_size);
		box.y_ker.Set(i, 1.0f / filter_size);
	}

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	t.start();
	auto out = qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, box, border_mode);
	t.end();

	test::PrintTime(t);

	// reread output image
	out = test::ReReadImage(out);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
	const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}
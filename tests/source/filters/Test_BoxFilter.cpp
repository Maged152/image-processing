#include "test_common.hpp"

TEST(Test_shakhbat_cv, BoxFilter)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// do the operation
	const size_t filter_x_size = 5;
	const size_t filter_y_size = 5;
	const bool normalize = true;
	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	t.Start();
	auto out = qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, filter_x_size, filter_y_size, normalize, border_mode);
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
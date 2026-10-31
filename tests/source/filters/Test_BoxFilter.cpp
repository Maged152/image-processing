#include "test_common.hpp"

TEST(Test_shakhbat_cv, BoxFilter)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// do the operation
	const int filter_x_size = 5;
	const int filter_y_size = 5;
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
	const bool load_ref = ref.Read(folder_path + "result.jpg");
	EXPECT_EQ(load_ref, true);

	test::CompareImages(out, ref);
}

// The in-place overload is never exercised by the reference-based test above;
// lock it in by requiring byte-identical results with the allocating overload.
TEST(Test_shakhbat_cv, BoxFilter_InPlace)
{
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.Read(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	const int filter_x_size = 5;
	const int filter_y_size = 5;
	const bool normalize = true;
	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// allocating result
	auto ref_out = qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, filter_x_size, filter_y_size, normalize, border_mode);

	// in-place on a buffer seeded with different content
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out(ref_out.Width(), ref_out.Height());
	out = in;

	qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, out, filter_x_size, filter_y_size, normalize, border_mode);

	test::CompareImages(out, ref_out);
}
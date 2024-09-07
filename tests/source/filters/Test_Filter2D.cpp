#include "test_common.hpp"

TEST(Test_shakhbat_cv, Filter2D)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/Filter2D/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

	// do the operation
	qlm::Kernel k{ 3, 3 };
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);

	t.Start();
	auto out = qlm::Filter2D(in, k, qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{});
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


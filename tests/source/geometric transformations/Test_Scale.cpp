#include "test_common.hpp"

TEST(Test_shakhbat_cv, Scale)
{
	qlm::Timer<qlm::usec> t_nn, t_bi,t_ci;
	const std::string folder_path = test::example_folder + "Geometric Transformations/Scale/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	float scale_x = 2.7f;
	float scale_y = 2.7f;

	t_nn.start();
	auto out_nn = qlm::Scale(in, qlm::InterpolationFlag::NEAREST_NEIGHBOR, scale_x, scale_y);
	t_nn.end();

	test::PrintTime(t_nn);

	t_bi.start();
	auto out_bi = qlm::Scale(in, qlm::InterpolationFlag::BILINEAR, scale_x, scale_y);
	t_bi.end();

	test::PrintTime(t_bi);

	t_ci.start();
	auto out_ci = qlm::Scale(in, qlm::InterpolationFlag::BICUBIC, scale_x, scale_y);
	t_ci.end();

	test::PrintTime(t_ci);


	// reread output image
	out_nn = test::ReReadImage(out_nn);
	out_bi = test::ReReadImage(out_bi);
	out_ci = test::ReReadImage(out_ci);

	// read reference image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref_nn, ref_bi, ref_ci;
	
	const bool load_ref_nn = ref_nn.LoadFromFile(folder_path + "nearest_neighbor.jpg");
	EXPECT_EQ(load_ref_nn, true);

	const bool load_ref_bi = ref_bi.LoadFromFile(folder_path + "bilinear.jpg");
	EXPECT_EQ(load_ref_bi, true);

	const bool load_ref_ci = ref_ci.LoadFromFile(folder_path + "bicubic.jpg");
	EXPECT_EQ(load_ref_ci, true);

	// compare results
	test::CompareImages(out_nn, ref_nn);
	test::CompareImages(out_bi, ref_bi);
	test::CompareImages(out_ci, ref_ci);
}
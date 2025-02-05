#include "test_common.hpp"

TEST(Test_shakhbat_cv, SeamCarving)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/SeamCarving/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;

	const int target_width = in.width - 500;
	const int target_height =  in.height + 200;

	const qlm::EnergyFlag energy = qlm::EnergyFlag::BACKWARD;
	const qlm::OrderFlag order = qlm::OrderFlag::WIDTH_FIRST;

	// do the operation
	t.Start();
	auto out = qlm::SeamCarving(in, target_width, target_height, energy, order);
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
#include "test_common.hpp"

TEST(Test_shakhbat_cv, GaussianPyramid)
{
	qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/GaussianPyramid/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

	// do the operation
	const int num_layers = 5;

    t.Start();
    auto pyr = qlm::GaussianPyramid(in, num_layers);
    t.End();

	test::PrintTime(t);

    for (int layer = 0; layer < num_layers; layer++)
    {
        // reread output image
	    auto out = test::ReReadImage(pyr.layers[layer]);

        // read reference image
        const std::string out_name = "layer_" + std::to_string(layer) + ".jpg";
        qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
        const bool load_ref = ref.LoadFromFile(folder_path + out_name);
        EXPECT_EQ(load_ref, true);

        test::CompareImages(out, ref);	
    }	
}
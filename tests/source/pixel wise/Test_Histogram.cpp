#include "test_common.hpp"

TEST(Test_shakhbat_cv, Histogram)
{
    qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	t.Start();
	auto out = qlm::Histogram(in);
	t.End();

	test::PrintTime(t);

    // reference result
    const auto ref = test::ReadHistogram<qlm::ImageFormat::RGB, uint8_t>("tests/source/pixel wise/result_histogram.txt");

    // compare results
    for (int c = 0; c < out.num_channels; c++)
    {
        for (int e = 0; e < out.tot_elements; e++)
        {
            EXPECT_EQ(out.hist[c][e], ref.hist[c][e]);
        }
    }
}
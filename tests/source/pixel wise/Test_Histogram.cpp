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

TEST(Test_shakhbat_cv, Histogram_Synthetic)
{
	// constant 4x4 GRAY image: all N pixels in one bin
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(4, 4);
	for (int i = 0; i < 16; i++)
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(100));

	auto out = qlm::Histogram(in);

	for (int e = 0; e < out.tot_elements; e++)
		EXPECT_EQ(out.hist[0][e], e == 100 ? 16 : 0);

	// cumsum: bins below 100 are 0, bins at/above 100 are 16
	auto cum = out.CumulativeHistogram();
	for (int e = 0; e < out.tot_elements; e++)
		EXPECT_EQ(cum.hist[0][e], e < 100 ? 0 : 16);

	// reset zeroes everything
	out.Reset();
	for (int e = 0; e < out.tot_elements; e++)
		EXPECT_EQ(out.hist[0][e], 0);
}
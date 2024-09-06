#include "test_common.h"

TEST(Test_shakhbat_cv, HarrisCorner)
{
    qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Resize/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");

    auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

    t.start();
    auto [min_loc, max_loc] = qlm::MinMaxLoc(gray);    
    t.end();

    test::PrintTime(t);

    // reference results
    constexpr int num_min = 42;
    constexpr int x_min = 246;
    constexpr int y_min = 338;

    constexpr int num_max = 1;
    constexpr int x_max = 354;
    constexpr int y_max = 139;

    // compare results
    EXPECT_EQ(num_min, min_loc.size());
    EXPECT_EQ(x_min, min_loc[0].x);
    EXPECT_EQ(y_min, min_loc[0].y);

    EXPECT_EQ(num_max, max_loc.size());
    EXPECT_EQ(x_max, max_loc[0].x);
    EXPECT_EQ(y_max, max_loc[0].y);
}
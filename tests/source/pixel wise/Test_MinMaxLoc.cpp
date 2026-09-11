#include "test_common.hpp"

TEST(Test_shakhbat_cv, MinMaxLoc)
{
    qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Geometric Transformations/Resize/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

    auto gray = qlm::ColorConvert< qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

    t.Start();
    auto [min_loc, max_loc] = qlm::MinMaxLoc(gray);    
    t.End();

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

TEST(Test_shakhbat_cv, MinMaxLoc_Synthetic)
{
	// 3x2 image with known distinct min (5) and max (250) locations
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(3, 2);
	const uint8_t vals[6] = { 100, 5, 100, 250, 100, 250 };
	for (int i = 0; i < 6; i++)
		in.SetPixel(i, qlm::Pixel<qlm::ImageFormat::GRAY, uint8_t>(vals[i]));

	auto [min_loc, max_loc] = qlm::MinMaxLoc(in);

	ASSERT_EQ(min_loc.size(), 1u);
	EXPECT_EQ(min_loc[0].x, 1);
	EXPECT_EQ(min_loc[0].y, 0);

	ASSERT_EQ(max_loc.size(), 2u);
	EXPECT_EQ(max_loc[0].x, 0);
	EXPECT_EQ(max_loc[0].y, 1);
	EXPECT_EQ(max_loc[1].x, 2);
	EXPECT_EQ(max_loc[1].y, 1);
}
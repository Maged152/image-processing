#include "test_common.hpp"

TEST(Test_shakhbat_cv, GoodFeaturesToTrack)
{
    qlm::Timer<qlm::usec> t{};
    const std::string folder_path = test::example_folder + "Features Detection/GoodFeaturesToTrack/";

    // read input image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    const bool load_in = in.Read(folder_path + "input.jpg");
    EXPECT_EQ(load_in, true);

    // Convert the input image to grayscale.
    const auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

    const int max_corners = 200;
    const double quality_level = 0.01;
    const double min_distance = 10.0;
    const int blockSize = 3;
    const int gradientSize = 3;
    const bool useHarrisDetector = true;
    const double k = 0.04;

    // do the operation
    t.Start();
    auto out = qlm::GoodFeaturesToTrack(gray, max_corners, quality_level, min_distance, blockSize, gradientSize, useHarrisDetector, k);
    t.End();

    // draw corners
    qlm::Circle<float> circle = { .radius = 3 };
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{255, 0, 255};

    for (auto& i : out)
    {
        circle.center = i.point;
        in = qlm::DrawCircle(in, circle, color);
    }

    test::PrintTime(t);

    // reread output image
    in = test::ReReadImage(in);

    // read reference image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
    const bool load_ref = ref.Read(folder_path + "result.jpg");
    EXPECT_EQ(load_ref, true);

    test::CompareImages(in, ref);
}

TEST(Test_shakhbat_cv, GoodFeaturesToTrack_MaxOne)
{
    const std::string folder_path = test::example_folder + "Features Detection/GoodFeaturesToTrack/";

    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    ASSERT_TRUE(in.Read(folder_path + "input.jpg"));
    const auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

    // max_corners=1 returns exactly the single strongest corner
    auto one = qlm::GoodFeaturesToTrack(gray, 1, 0.01, 10.0, 3, 3, true, 0.04);
    ASSERT_EQ(one.size(), 1);

    // it must agree with the top of the full deterministic ordering
    auto many = qlm::GoodFeaturesToTrack(gray, 200, 0.01, 10.0, 3, 3, true, 0.04);
    ASSERT_FALSE(many.empty());
    EXPECT_EQ(one[0].point.x, many[0].point.x);
    EXPECT_EQ(one[0].point.y, many[0].point.y);
    EXPECT_FLOAT_EQ(one[0].response, many[0].response);
}


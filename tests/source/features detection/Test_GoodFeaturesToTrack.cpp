#include "test_common.hpp"

TEST(Test_shakhbat_cv, GoodFeaturesToTrack)
{
    qlm::Timer<qlm::usec> t{};
    const std::string folder_path = test::example_folder + "Features Detection/GoodFeaturesToTrack/";

     /*
    // Load the input image.
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }

    // Check alpha component.
    bool alpha{ true };
    if (in.NumerOfChannels() == 3)
        alpha = false;

    // Detect corners with GoodFeaturesToTrack.
    const int max_corners = 200;
    const double quality_level = 0.01;
    const double min_distance = 10.0;
    const int blockSize = 3;
    const int gradientSize = 3;
    const bool useHarrisDetector = true;
    const double k = 0.04;

    t.Start();
    auto corners = qlm::GoodFeaturesToTrack(gray, max_corners, quality_level, min_distance, blockSize, gradientSize, useHarrisDetector, k);
    t.End();


    // Draw the detected corners on the input image.
    qlm::Circle<int> circle{ .radius = 3 };
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{255, 0, 255};

    for (const auto& corner : corners)
    {
        circle.center = corner.point;
        in = qlm::DrawCircle(in, circle, color);
    }

    if (!in.SaveToFile("result.jpg", alpha))
    {
        std::cout << "Failed to write \n";
    }
}
     
     */
    // read input image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
    EXPECT_EQ(load_in, true);

    // Convert the input image to grayscale.
    const auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

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
    qlm::Circle<int> circle = { .radius = 3 };
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{255, 0, 255};

    for (auto& i : out)
    {
        circle.center = i.point;
        in = qlm::DrawCircle(in, circle, color);
    }

    test::PrintTime(t);

    in.SaveToFile("result2.jpg", false);

    // reread output image
    in = test::ReReadImage(in);

    // read reference image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
    const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
    EXPECT_EQ(load_ref, true);

    test::CompareImages(in, ref);
}

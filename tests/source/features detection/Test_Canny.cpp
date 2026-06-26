#include "test_common.hpp"

TEST(Test_shakhbat_cv, Canny)
{
    qlm::Timer<qlm::usec> t{};
    const std::string folder_path = test::example_folder + "Features Detection/Canny/";

    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    const bool load_in = in.LoadFromFile(folder_path + "input.png");
    EXPECT_EQ(load_in, true);

    bool alpha{ true };
    if (in.NumerOfChannels() == 1)
        alpha = false;

    auto gray = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in);

    const int gaussian_size = 5;
    const float sigma = 1.4f;
    auto blurred = qlm::Gaussian(gray, gaussian_size, sigma);

    const int threshold_low = 50;
    const int threshold_high = 140;
    const int sobel_size = 3;
    const bool l2_gradient = true;

    t.Start();
    auto result = qlm::Canny(blurred, threshold_low, threshold_high, sobel_size, l2_gradient);
    t.End();

    test::PrintTime(t);

    auto output = test::ReReadImage(result);

    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> ref;
    const bool load_ref = ref.LoadFromFile(folder_path + "result.jpg");
    EXPECT_EQ(load_ref, true);

    test::CompareImages(output, ref);
}

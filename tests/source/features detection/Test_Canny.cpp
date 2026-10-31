#include "test_common.hpp"

TEST(Test_shakhbat_cv, Canny)
{
    qlm::Timer<qlm::usec> t{};
    const std::string folder_path = test::example_folder + "Features Detection/Canny/";

    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    const bool load_in = in.Read(folder_path + "input.png");
    EXPECT_EQ(load_in, true);

    bool alpha{ true };
    if (in.NumberOfChannels() == 1)
        alpha = false;

    auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

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
    const bool load_ref = ref.Read(folder_path + "result.jpg");
    EXPECT_EQ(load_ref, true);

    test::CompareImages(output, ref);
}

// The l2_gradient = false branch is not covered by the reference test above.
// A synthetic vertical step edge gives a hand-checkable edge map.
TEST(Test_shakhbat_cv, Canny_L1Gradient_Synthetic)
{
    // vertical step edge: left half dark, right half bright
    const int width = 64;
    const int height = 64;
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            in.SetPixel(x, y, uint8_t{ x < 32 ? 20u : 240u });
        }
    }

    // L1 magnitude branch; thresholds well above the flat-region noise (0).
    // BORDER_REFLECT keeps the constant background out of the gradient at the
    // image border (a zero border would mark the border ring itself as edges).
    const int threshold_low = 60;
    const int threshold_high = 150;
    const int filter_size = 3;
    const bool l2_gradient = false;
    qlm::BorderMode<qlm::ImageFormat::GRAY, uint8_t> border_mode;
    border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

    auto edges = qlm::Canny(in, threshold_low, threshold_high, filter_size, l2_gradient, border_mode);

    // only the step columns may contain edges; everything else must be 0 (or promoted weak = 255)
    int edge_count = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            const uint8_t v = edges.GetPixel(x, y).v;
            EXPECT_TRUE(v == 0 || v == 255);
            if (v == 255)
            {
                edge_count++;
                EXPECT_GE(x, 30);
                EXPECT_LE(x, 33);
            }
        }
    }

    EXPECT_GE(edge_count, height / 2);  // at least half a column of edges
    EXPECT_LE(edge_count, 4 * height);  // at most a few columns wide
}

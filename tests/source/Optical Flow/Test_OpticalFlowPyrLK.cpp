#include "test_common.hpp"

TEST(Test_shakhbat_cv, OpticalFlowPyrLK)
{
    qlm::Timer<qlm::msec> t{};
    const std::string folder_path = test::example_folder + "Optical Flow/OpticalFlowPyrLK/";

    // the three test image sets
    const std::string sets[3] = { "MiniCooper", "Urban2", "Walking" };

    // a different parameter set for each image to cover the API
    const qlm::Size win_sizes[3] = { { 21, 21 }, { 15, 15 }, { 11, 11 } };
    const int max_levels[3] = { 3, 2, 5 };
    const qlm::TermCriteria criterias[3] = { { 30, 0.01 }, { 20, 0.001 }, { 10, 0.1 } };
    // MiniCooper & Urban2 : default threshold (gate inactive)
    // Walking             : OpenCV-equivalent threshold in raw units (1e-4 * 2^20) -> gate active
    const double min_eig_thresholds[3] = { 1e-4, 1e-4, 1e-4 * (1 << 20) };

    for (int s = 0; s < 3; s++)
    {
        const std::string& set = sets[s];
        std::cout << "---- " << set << " ----\n";

        const std::string frame0 = folder_path + set + "_frame0.png";
        const std::string frame1 = folder_path + set + "_frame1.png";

        // Load the input images.
        qlm::Image<qlm::ImageFormat::GRAY, uint8_t> prev_img, next_img;
        const bool load_prev = prev_img.LoadFromFile(frame0);
        EXPECT_EQ(load_prev, true);

        const bool load_next = next_img.LoadFromFile(frame1);
        EXPECT_EQ(load_next, true);

        // ============================================================
        // Detect corners with GoodFeaturesToTrack
        // ============================================================

        const int max_corners = 200;
        const double quality_level = 0.01;
        const double min_distance = 10.0;
        const int blockSize = 3;
        const int gradientSize = 3;
        const bool useHarrisDetector = true;
        const double k = 0.04;

        auto prev_corners = qlm::GoodFeaturesToTrack(
            prev_img,
            max_corners,
            quality_level,
            min_distance,
            blockSize,
            gradientSize,
            useHarrisDetector,
            k
        );

        EXPECT_GT(prev_corners.size(), 0);

        // ============================================================
        // Lucas-Kanade Optical Flow
        // ============================================================

        const qlm::Size win_size = win_sizes[s];
        const int max_level = max_levels[s];
        const qlm::TermCriteria criteria = criterias[s];
        const double min_eig_threshold = min_eig_thresholds[s];
        std::vector<qlm::KeyPoint<float>> initial_guess;

        t.Start();

        auto next_corners = qlm::OpticalFlowPyrLK(
            prev_img,
            next_img,
            prev_corners,
            initial_guess,
            win_size,
            max_level,
            criteria,
            min_eig_threshold
        );

        t.End();

        test::PrintTime(t);

        // ============================================================
        // Validate the output
        // ============================================================

        // the output must keep one key-point per input key-point
        ASSERT_EQ(next_corners.size(), prev_corners.size());

        
        // ============================================================
        // Draw optical flow
        //
        // Only key-points with a displacement larger than the flow
        // threshold are drawn.
        // ============================================================

        const float flow_threshold = 1.0f; // px

        qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0 };
        qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> red{ 0, 0, 255 };

        auto next_rgb = qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(next_img);

        for (int i = 0; i < next_corners.size(); i++)
        {
            if (next_corners[i].status == qlm::KPStatusFlag::TRACKED)
            {
                const qlm::Point<float> displacement = next_corners[i].point - prev_corners[i].point;

                // keep only key-points with meaningful motion
                if (displacement.x * displacement.x + displacement.y * displacement.y < flow_threshold * flow_threshold)
                {
                    continue;
                }

                const qlm::Line l =
                {
                    int(prev_corners[i].point.x),
                    int(prev_corners[i].point.y),
                    int(next_corners[i].point.x),
                    int(next_corners[i].point.y)
                };

                // Green arrow from previous point to tracked point.
                next_rgb = qlm::DrawArrowedLine(next_rgb, l, green, 0.3);

                // Red filled dot at tracked point.
                qlm::Circle<int> endpoint
                {
                    .center =
                    {
                        int(next_corners[i].point.x),
                        int(next_corners[i].point.y)
                    },
                    .radius = 2
                };

                next_rgb = qlm::DrawCircle(next_rgb, endpoint, red, -1);
            }
        }

        // reread output image
        next_rgb = test::ReReadImage(next_rgb, "png");

        // read reference image
        qlm::Image<qlm::ImageFormat::RGB, uint8_t> ref;
        const bool load_ref = ref.LoadFromFile(folder_path + set + "_result.png");
        EXPECT_EQ(load_ref, true);

        test::CompareImages(next_rgb, ref);
    }
}

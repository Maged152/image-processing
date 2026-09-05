#include "shakhbat_cv.hpp"

int main()
{
    std::cout << "start example\n";

    qlm::Timer<qlm::msec> t{};
    std::string frame0 = "./doc/Functions/Optical Flow/OpticalFlowPyrLK/Walking_frame0.png";
    std::string frame1 = "./doc/Functions/Optical Flow/OpticalFlowPyrLK/Walking_frame1.png";

    // Load the input images.
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> prev_img, next_img;
    if (!prev_img.LoadFromFile(frame0))
    {
        std::cout << "Failed to read frame0\n";
        return -1;
    }

    if (!next_img.LoadFromFile(frame1))
    {
        std::cout << "Failed to read frame1\n";
        return -1;
    }

    // Check alpha component.
    bool alpha{ true };
    if (prev_img.NumerOfChannels() == 3)
        alpha = false;

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

    std::cout << "Found " << prev_corners.size() << " corners\n";

    // ============================================================
    // Lucas-Kanade Optical Flow
    // ============================================================

    const qlm::Size win_size = { 11, 11 };
    const int max_level = 5;
    const qlm::TermCriteria criteria = { 10, 0.1 };
    const double min_eig_threshold = 1e-4 * (1 << 20);
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

    std::cout << "Time = " << t.ElapsedString() << "\n";

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

    // ============================================================
    // Save result image
    // ============================================================

    if (!next_rgb.SaveToFile("Walking_result.png", alpha))
    {
        std::cout << "Failed to write Walking_result.png\n";
    }
    else
    {
        std::cout << "Saved Walking_result.png\n";
    }
}

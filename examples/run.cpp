#include "shakhbat_cv.hpp"
#include <fstream>

int main()
{
    std::cout << "start example\n";

    qlm::Timer<qlm::msec> t{};
    std::string frame0 = "other-data-gray/MiniCooper/frame10.png";
    std::string frame1 = "other-data-gray/MiniCooper/frame11.png";

    // Load the input image.
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

    const qlm::Size wind_size = {21, 21};
    const int max_level = 3;
    const qlm::TermCriteria criteria = {30, 0.01};
    std::vector<qlm::KeyPoint<float>> init_guess;

    t.Start();

    auto next_corners = qlm::OpticalFlowPyrLK(
        prev_img,
        next_img,
        prev_corners,
        init_guess,
        wind_size,
        max_level,
        criteria
    );

    t.End();

    std::cout << "Time = " << t.ElapsedString() << "\n";
    std::cout << "Found next_corners " << next_corners.size() << " corners\n";


    // ============================================================
    // Write optical flow results to CSV
    //
    // Format:
    // frame0_x,frame0_y,frame1_x,frame1_y
    // ============================================================

    std::ofstream csv("optical_flow_cpp.csv");

    if (!csv.is_open())
    {
        std::cout << "Failed to open optical_flow_cpp.csv\n";
        return -1;
    }

    csv << "frame0_x,frame0_y,frame1_x,frame1_y\n";

    // Use the same precision as the Python/OpenCV reference.
    csv << std::fixed << std::setprecision(8);

    for (size_t i = 0; i < next_corners.size(); ++i)
    {
        if (next_corners[i].status == qlm::KPStatusFlag::TRACKED)
        {
            csv << prev_corners[i].point.x << ","
                << prev_corners[i].point.y << ","
                << next_corners[i].point.x << ","
                << next_corners[i].point.y << "\n";
        }
    }

    csv.close();

    std::cout << "Saved optical_flow_cpp.csv\n";


    // ============================================================
    // Draw optical flow
    // ============================================================

    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> green{0, 255, 0};
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> red{0, 0, 255};

    auto next_rgb = qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(next_img);

    for (int i = 0; i < next_corners.size(); i++)
    {
        if (next_corners[i].status == qlm::KPStatusFlag::TRACKED)
        {
            const qlm::Point<float> displacement = next_corners[i].point - prev_corners[i].point;

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

    if (!next_rgb.SaveToFile("result_cpp.jpg", alpha))
    {
        std::cout << "Failed to write result_cpp.jpg\n";
    }
    else
    {
        std::cout << "Saved result_cpp.jpg\n";
    }
}
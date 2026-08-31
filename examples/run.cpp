#include "shakhbat_cv.hpp"

int main()
{
    std::cout << "start example\n";

    qlm::Timer<qlm::msec> t{};
    std::string frame0 = "other-data-gray/Beanbags/frame10.png";
    std::string frame1 = "other-data-gray/Beanbags/frame11.png";

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

    // Detect corners with GoodFeaturesToTrack.
    const int max_corners = 200;
    const double quality_level = 0.01;
    const double min_distance = 10.0;
    const int blockSize = 3;
    const int gradientSize = 3;
    const bool useHarrisDetector = true;
    const double k = 0.04;

    auto prev_corners = qlm::GoodFeaturesToTrack(prev_img, max_corners, quality_level, min_distance, blockSize, gradientSize, useHarrisDetector, k);

    std::cout << "Found " << prev_corners.size() << " corners\n";


    const qlm::Size wind_size = {21, 21};
    const int max_level = 3;
    const qlm::TermCriteria criteria = {30, 0.01};
    std::vector<qlm::KeyPoint<float>> init_guess;

    t.Start();
    auto next_corners = qlm::OpticalFlowPyrLK(prev_img, next_img,
                                              prev_corners, init_guess,
                                              wind_size, max_level, criteria);
    t.End();

    
    std::cout << "Time = " << t.ElapsedString() << "\n";

    std::cout << "Found next_corners " << next_corners.size() << " corners\n";

    // draw optical flow
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> green{0, 255, 0};
    qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> red{0, 0, 255};
    auto next_rgb = qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(next_img);

    for (int i = 0; i < next_corners.size(); i++)
    {
        if (next_corners[i].status == qlm::KPStatusFlag::TRACKED)
        {
            const qlm::Point<float> displacement = {next_corners[i].point - prev_corners[i].point};

            const qlm::Line l = {int(prev_corners[i].point.x), int(prev_corners[i].point.y), 
                                 int(next_corners[i].point.x), int(next_corners[i].point.y)};

            // green arrow from the previous point to the tracked point (tipLength matches Python's 0.3)
            next_rgb = qlm::DrawArrowedLine(next_rgb, l, green, 0.3);

            // red filled dot at the tracked point (matches Python's cv2.circle(..., -1))
            qlm::Circle<int> endpoint{ .center = {int(next_corners[i].point.x), int(next_corners[i].point.y)}, .radius = 2 };
            next_rgb = qlm::DrawCircle(next_rgb, endpoint, red, -1);
        }
    }

    if (!next_rgb.SaveToFile("result.jpg", alpha))
    {
        std::cout << "Failed to write \n";
    }
}


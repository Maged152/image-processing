#include "shakhbat_cv.hpp"

int main()
{
    qlm::Timer<qlm::usec> t{};
    std::string file_name = "input.jpg";
    // load the image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    if (!in.Read(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }
    auto gray = qlm::ColorConvert<qlm::ImageFormat::GRAY>(in);

    auto gx = qlm::ScharrX<uint8_t, int16_t>(gray);
    auto gy = qlm::ScharrY<uint8_t, int16_t>(gray);

    t.Start();
    auto mag_l2 = qlm::Magnitude(gx, gy, false);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";

	// convert bit depth to u8 for viewing
	auto view_l2 = qlm::ConvertBitDepth<qlm::ImageFormat::GRAY, uint16_t, uint8_t>(mag_l2);

    view_l2.Write("result_l2.jpg");
}


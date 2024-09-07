#include "shakhbat_cv.hpp"
#include <iostream>

int main()
{
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "castle.jpg"; 
    // load the input image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the input image\n";
        return -1;
    }

    const size_t target_width = in.width - 200;
    const size_t target_height = in.height - 100;

    const qlm::EnergyFlag energy = qlm::EnergyFlag::BACKWARD;
    const qlm::OrderFlag order = qlm::OrderFlag::HEIGHT_FIRST;

    t.Start();
    auto out = qlm::SeamCarving(in, target_width, target_height, energy, order);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";

    out.SaveToFile("out.jpg");
}

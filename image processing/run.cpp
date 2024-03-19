#include "shakhbat.h"
#include "test/test.h"

int main()
{
    //test::Test_All();
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg"; 
    // load the image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }
    // check alpha component
    bool alpha{ true };
    if (in.NumerOfChannels() == 1)
        alpha = false;

    const unsigned int k = 4;
    // do the operation
    t.start();
    auto clusters = qlm::KMeans(in, k);
    t.end();

    t.show();

    // out image to draw on
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> out;
    out.create(in.Width(), in.Height());

    for (int c = 0; c < clusters.size(); c++)
    {
        auto& color = clusters[c].color;

        for (auto& idx : clusters[c].pixels)
        {
            out.SetPixel(idx.x, idx.y, color);
        }
    }

   

    if (!out.SaveToFile("result.jpg", alpha))
    {
        std::cout << "Failed to write \n";
    }

    
}


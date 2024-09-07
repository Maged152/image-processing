#include "test_common.hpp"
#include <fstream>

template <typename T, std::size_t N, std::size_t M>
void readArrayFromFile(const std::string& filename, std::array<std::array<T, N>, M>& out)
{
    std::ifstream file(filename, std::ios::binary);
    
    if (!file)
    {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    for (auto& channel : out)
    {
        file.read(reinterpret_cast<char*>(channel.data()), channel.size() * sizeof(T));
    }

    file.close();
    if (file.fail())
    {
        std::cerr << "Failed to read data from file: " << filename << std::endl;
    }
}

TEST(Test_shakhbat_cv, Histogram)
{
    qlm::Timer<qlm::usec> t{};
	const std::string folder_path = test::example_folder + "Filters/BoxFilter/";

	// read input image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	const bool load_in = in.LoadFromFile(folder_path + "input.jpg");
	EXPECT_EQ(load_in, true);

	t.Start();
	auto out = qlm::Histogram(in);
	t.End();

	test::PrintTime(t);

    // reference result
    qlm::Histogram_t<qlm::ImageFormat::RGB, uint8_t> ref;
    readArrayFromFile("tests/source/pixel wise/result_hist.txt", ref.hist);

    // compare results
    for (int c = 0; c < out.num_channels; c++)
    {
        for (int e = 0; e < out.tot_elements; e++)
        {
            EXPECT_EQ(out.hist[c][e], ref.hist[c][e]);
        }
    }
}
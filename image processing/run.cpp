#include "shakhbat.h"

int main()
{
	qlm::Timer<qlm::msec> t{};
	std::string file_name = "images.png";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;

	unsigned int filter_size = 3;
	unsigned int block_size = 3;
	float k = 0.04;
	float threshold = 400;
	
	// do the operation
	t.start();
	auto out = qlm::HarrisCorner(in, block_size, filter_size, k, threshold);
	t.end();

	t.show();

	qlm::Circle<int> circle = { .center = {240,240}, .radius = 2 };
	qlm::Pixel <qlm::ImageFormat::RGB, uint8_t> green{ 0, 255, 0};

	for (auto& i : out)
	{
		circle.center = i.point;
		qlm::DrawCircle(in, circle, green);
	}

	
	if (!in.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}

}


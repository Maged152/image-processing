#include "shakhbat.h"


int main()
{
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
	if (in.NumerOfChannels() == 3)
		alpha = false;

	float angle = 30.0f;
	float scale = 1.0f;
	int centre_x = (in.Width() - 1) / 2;
	int centre_y = (in.Height() - 1) / 2;
	qlm::Point<int> centre{ centre_x, centre_y };
	// background color
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> pix{ 255, 0, 0 };
	// do the operation
	t.start();
	auto out = qlm::WrapRotate(in, angle, centre, scale, pix);
	t.end();

	t.show();

	
	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
}


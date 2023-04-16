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
	
	qlm::Kernel1D ker{ 3 };
	ker.Set(0, 1.0 / 3); ker.Set(1, 1.0 / 3); ker.Set(2, 1.0 / 3);


	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::SepFilter2D(in, ker, ker, qlm::Border::BORDER_REFLECT);
	t.end();
	
	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
}
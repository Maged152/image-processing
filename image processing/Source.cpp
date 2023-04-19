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
	


	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> out = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, 
																		qlm::ImageFormat::GRAY, uint8_t>(in);
	t.end();
	
	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
}
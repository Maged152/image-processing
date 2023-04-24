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
	

	// RGB to YCrCb
	t.start();
	auto out = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, 
								 qlm::ImageFormat::YCrCb, uint8_t>(in);
	t.end();
	
	t.show();

	
	// YCrCb2RGB
	auto out2 = qlm::ColorConvert<qlm::ImageFormat::YCrCb, uint8_t,
								  qlm::ImageFormat::RGB, uint8_t>(out);


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}

	if (!out2.SaveToFile("result2.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
}
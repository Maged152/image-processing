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

	qlm::Kernel k{ 3, 3 };
	// sharpen filter
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);

	// do the operation
	t.start();
	auto out = qlm::Filter2D(in, k, qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{});
	t.end();

	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}

}


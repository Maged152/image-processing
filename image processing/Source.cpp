#include "shakhbat.h"


int main()
{
	
	qlm::Timer<msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	
	qlm::Kernel k{ 3, 3 };
	// sharpen filter
	k.Set(0, 0, 1.0 / 9); k.Set(0, 1, 1.0 / 9); k.Set(0, 2, 1.0 / 9);
	k.Set(1, 0, 1.0 / 9); k.Set(1, 1, 1.0 / 9); k.Set(1, 2, 1.0 / 9);
	k.Set(2, 0, 1.0 / 9); k.Set(2, 1, 1.0 / 9); k.Set(2, 2, 1.0 / 9);

	qlm::Kernel1D k2{ 3 };
	k2.Set(0, 1.0/3); k2.Set(1, 1.0 / 3); k2.Set(2, 1.0 / 3);

	sf::Image out{ in };
	sf::Image out2{ in };
	// do the operation
	t.start();
	qlm::Filter2D(in, out, k);
	qlm::SepFilter2D(in, out2, k2, k2);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");
	out2.saveToFile("result2.jpg");
	
	

	for (unsigned int x = 0; x < out.getSize().x; x++)
	{
		for (unsigned int y = 0; y < out.getSize().y; y++)
		{
			if (std::abs(out.getPixel(x, y).r - out2.getPixel(x, y).r) > 1)
			{
				std::cout << " error" << std::endl;
			}
		}
	}
}
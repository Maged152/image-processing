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

	qlm::Kernel1D ker{ 3 };
	ker.Set(0, 1.0/3); ker.Set(1, 1.0 / 3); ker.Set(2, 1.0 / 3);

	sf::Image out{ in };
	// do the operation
	t.start();
	qlm::SepFilter2D(in, out, ker, ker, qlm::BORDER::BORDER_REFLECT);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");
	
}
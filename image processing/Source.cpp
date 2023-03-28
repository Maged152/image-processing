#include "shakhbat.h"


int main()
{

	qlm::Timer<msec> t{};
	std::string file_name = "testimg.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	qlm::Kernel k{3, 3};
	// sharpen filter
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);
	

	sf::Image out{in};
	// do the operation
	t.start();
	qlm::Filter2D(in, out, k, qlm::BORDER::BORDER_REFLECT);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");

}
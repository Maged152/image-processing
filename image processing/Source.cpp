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
	unsigned int width = 960u; 
	unsigned int height = 540u;
	

	sf::Image out;
	// do the operation
	t.start();
	qlm::Resize(in, out, width, height);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");

}
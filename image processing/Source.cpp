#include "Timer.h"
#include "Scale/Scale.h"


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

	sf::Image out;
	// do the operation
	t.start();
	qlm::Scale(in, out, qlm::ScaleMethod::NEAREST_NEIGHBOR, 2.7, 2.7);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");

}
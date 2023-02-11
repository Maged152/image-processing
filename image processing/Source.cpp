#include "Timer.h"
#include "Scale_function/Scale.h"


int main()
{
	Timer t{};
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
	Scale(in, out, ScaleMethod::NEAREST_NEIGHBOR, 1.5, 1.5);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");

}
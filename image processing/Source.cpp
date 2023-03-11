#include "shakhbat.h"


int main()
{
	qlm::Timer<msec> t{};
	std::string file_name = "test.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	sf::Image out;
	std::vector<qlm::LinePolar> lines;
	// do the operation
	t.start();
	qlm::HoughLines(in, lines, 1, 3.14f / 180, 150);
	t.end();

	t.show();

	for (auto& line : lines)
	{
		qlm::DrawLine(in, line, sf::Color{ 0,255,0 });
	}
	// Save the image to a file
	out.saveToFile("result.jpg");
}
#include "shakhbat.h"


int main()
{
	qlm::Timer<msec> t{};
	std::string file_name = "in.png";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	std::vector<qlm::LinePolar> lines;
	// do the operation
	t.start();
	qlm::HoughLines(in, lines, 1, 3 * 3.14f / 180, 110);
	t.end();

	t.show();
	std::cout << lines.size() << "\n";
	for (auto& line : lines)
	{
		std::cout << line.radius << "  " << line.angle << "\n";
		qlm::DrawLine(in, line, sf::Color{ 0,255,0 });
	}
	// Save the image to a file
	in.saveToFile("result.jpg");
}
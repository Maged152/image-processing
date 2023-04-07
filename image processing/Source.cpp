#include "shakhbat.h"


int main()
{
	qlm::Timer<msec> t{};
	std::string file_name = "input.png";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	// do the operation
	t.start();
	std::vector<qlm::LinePolar> lines = qlm::HoughLines(in, 1, 3 * 3.14f / 180, 110);
	t.end();

	t.show();
	std::cout <<"number of lines : " << lines.size() << "\n";
	for (auto& line : lines)
	{
		std::cout << "r: " << line.radius << "  theta: " << line.angle << "\n";
		qlm::DrawLine(in, line, sf::Color{ 0,255,0 });
	}
	// Save the image to a file
	in.saveToFile("result.jpg");
	
}
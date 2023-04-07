# Hough Lines

## Description
Finds lines in a binary image using the standard Hough transform
## C++ API
```c++
namespace qlm
{
	std::vector<LinesPolar> HoughLines(
		const sf::Image& in, 
			  float  rho, 
			  float theta_step,
			  int threshold, 
			  double min_theta = 0, 
			  double max_theta = std::numbers::pi
		);
}
```
```c++
namespace qlm
{
	struct LinesPolar
	{
		int radius;
		int angle;
	};
}
```
## Parameters

| Name        | Type         | Description                                                                                  |
|-------------|--------------|----------------------------------------------------------------------------------------------|
| `in`        | `sf::Image`  | The input image.                                                                             |
| `rho`       | `float`      | The Distance resolution of the accumulator in pixels.                                        |
| `theta`     | `float`      | The angle resolution of the accumulator in radians.                                          |
| `threshold` | `int`        | The threshold parameter. Only those lines are returned that get enough votes ( >threshold ). |
| `min_theta` | `double`     | The minimum angle to check for lines. Must fall between 0 and max_theta.                     |
| `max_theta` | `double`     | The upper bound for the angle. Must fall between min_theta and PI.                         |

## Example 
	rho = 1
	theta = 3 * PI / 180
	threshold = 110
	min_theta = 0
	double max_theta = PI

```c++
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
```

### The input
![Input Image](input.png)
### The output
![Input Image](result.jpg)

Time = 2 ms


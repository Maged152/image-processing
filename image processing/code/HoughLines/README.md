# Hough Lines

## Description
Finds lines in a binary image using the standard Hough transform
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<LinePolar> HoughLines(const Image<frmt, T>& in,
									  float  rho, 
									  float theta_step, 
									  int threshold,
									  double min_theta = 0,
									  double max_theta = std::numbers::pi);
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
| `in`        | `Image`      | The input image.                                                                             |
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
    qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.png";
	// load the image
	qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 1)
		alpha = false;
	// do the operation
	t.start();
	std::vector<qlm::LinePolar> lines = qlm::HoughLines(in, 1, 3 * 3.14f / 180, 110);
	t.end();

	t.show();

	// out image to draw on
	auto draw = qlm::ColorConvert< qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>(in);

	for (auto& line : lines)
	{
		draw = qlm::DrawLine(draw, line, qlm::Pixel<qlm::ImageFormat::RGB, uint8_t>{ 0, 255, 0 });
	}

	if (!draw.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}

```

### The input
![Input Image](input.png)
### The output
![Input Image](result.jpg)

Time = 2 ms


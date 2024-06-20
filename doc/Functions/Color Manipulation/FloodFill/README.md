# Flood Fill

## Description
The function performs region filling starting from certain point with a given color.

You can check the implementation [here](../../../../source/FloodFill.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> FloodFill(
		const Image<frmt, T>& in,
		const Point<int>& seed_point,
		const Pixel<frmt, T>& fill_color,
		const Pixel<frmt, T>& threshold = Pixel<frmt, T>{});
}
```

## Parameters

| Name         | Type         | Description                                                               |
|--------------|--------------|---------------------------------------------------------------------------|
| `in`         | `Image`      | The input image.                                                          |
| `seed_point` | `Point<int>` | seed point location.                                                      |
| `fill_color` | `Pixel`      | the color to fill the area.                                               |
| `threshold`  | `Pixel`      | the threshold which determines if the point brlongs to the region or not. |

## Return Value
The function returns an image of type `Image<frmt, T>`.

## Example

```c++
	qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;

	qlm::Point seed{ 300 ,300 };
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> color{0,0,0};
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> threshold{ 20,20,20 };
	// do the operation
	t.start();
	auto out = qlm::FloodFill(in, seed, color, threshold);
	t.end();

	t.show();


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 5 ms

# Resize

## Description
The function resizes the input image to the new given dimensions

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Resize(
		const Image<frmt, T>& in,
		const unsigned int width,
		const unsigned int height,
		const InterpolationFlag method = InterpolationFlag::BILINEAR,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```
```c++
namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	}
}
```
## Parameters

| Name           | Type           | Description                      |
|----------------|----------------|----------------------------------|
| `in`           | `Image`        | The input image.                 |
| `width`        | `unsigned int` | The new width.                   |
| `height`       | `unsigned int` | The new height.                  |
| `method`       | `ScaleMethod`  | The scale method.                |
| `border_mode`  | `BorderMode`   | The pixel extrapolation method.  |

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
	
	unsigned int width = 960u;
	unsigned int height = 540u;

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::Resize(in, width, height);
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

Time = 23 ms

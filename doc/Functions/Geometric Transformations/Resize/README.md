# Resize

## Description
The function resizes the input image to the new given dimensions

You can check the implementation [here](../../../../source/Resize.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Resize(
		const Image<frmt, T>& in,
		const int width,
		const int height,
		const InterpolationFlag method = InterpolationFlag::BILINEAR,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name           | Type           | Description                      |
|----------------|----------------|----------------------------------|
| `in`           | `Image`        | The input image.                 |
| `width`        | `int`          | The new width.                   |
| `height`       | `int`          | The new height.                  |
| `method`       | `InterpolationFlag` | The interpolation method.       |
| `border_mode`  | `BorderMode`   | The pixel extrapolation method.  |

## Return Value
The function returns an image of type `Image<frmt, T>`.

## Example

```c++
    qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.Read(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumberOfChannels() == 3)
		alpha = false;
	
	const int width = 960;
	const int height = 540;

	// do the operation
	t.Start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::Resize(in, width, height);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";
	
	if (!out.Write("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 23 ms
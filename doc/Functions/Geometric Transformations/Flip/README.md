# Flip

## Description
The function flips an image around a given axis.

You can check the implementation [here](../../../../source/Flip.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Flip(
		const Image<frmt, T>& in,
		const FlipFlag flip_code);
}
```

## Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| `in`        | `Image`    | The input image.                                                  |
| `flip_code` | `FlipFlag` | An enum to specify how to flip the image.                         |

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

	// do the operation
	t.start();
	auto out = qlm::Flip(in, qlm::FlipFlag::FLIP_VERTICAL_AXIS);
	t.end();

	t.show();


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 1 ms
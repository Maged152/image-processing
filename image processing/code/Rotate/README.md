# Rotate

## Description
The function rotates an image  in multiples of 90 degrees.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(
		const Image<frmt, T>& in,
		const RotateFlag rotate_code);
}
```
```c++
namespace qlm
{
	enum class RotateFlag
	{
		ROTATE_90_CLOCKWISE,
		ROTATE_180,
		ROTATE_90_COUNTERCLOCKWISE,
	};
}
```
## Parameters

| Name          | Type         | Description                                                         |
|---------------|--------------|---------------------------------------------------------------------|
| `in`          | `Image`      | The input image.                                                    |
| `rotate_code` | `RotateFlag` | An enum to specify how to rotate the image.                         |

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
	auto out = qlm::Rotate(in, qlm::RotateFlag::ROTATE_90_CLOCKWISE);
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


# Transpose

## Description
The function transposes the input image.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(
		const Image<frmt, T>& in);
}
```


## Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| `in`        | `Image`    | The input image.                                                  |

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
	auto out = qlm::Transpose(in);
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

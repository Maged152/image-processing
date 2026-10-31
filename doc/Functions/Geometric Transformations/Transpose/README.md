# Transpose

## Description
The function transposes the input image.

You can check the implementation [here](../../../../source/Transpose.cpp)

## C++ API

### Allocating overload
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(const Image<frmt, T>& in);
}
```
### In-place overload
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	void Transpose(const Image<frmt, T>& in, Image<frmt, T>& out);
}
```


## Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| `in`        | `Image`    | The input image.                                                  |
| `out`       | `Image`    | The output image (in-place overload only). Must have dimensions `h x w` when `in` is `w x h`. |

## Return Value
The function returns an image of type `Image<frmt, T>` (allocating overload).

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

	// do the operation
	t.Start();
	auto out = qlm::Transpose(in);
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

Time = 1 ms

# Filter 2D

## Description
Applys 2D filter of size MxN on the input image
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		      Border border_type = Border::BORDER_CONSTANT,
		      int border_value = 0
	);
}
```
```c++
namespace qlm
{
	enum class BORDER
	{
		BORDER_CONSTANT,
		BORDER_REPLICATE,
		BORDER_REFLECT,
	};
}
```
## Parameters

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                             |
| `kernel`       | `Kernel`     | The kernel to be applied on the input image.                                                 |
| `border_type`  | `BORDER`     | The pixel extrapolation method.                                                              |
| `border_value` | `int`        | The value to be used if the border is BORDER::BORDER_CONSTANT.                               |


## Example

	|0  -1  0|  
	|-1  5 -1|
	|0  -1  0|

	border = :BORDER::BORDER_REFLECT

```c++
namespace qlm
{
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
	
	qlm::Kernel k{ 3, 3 };
	// sharpen filter
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::Filter2D(in, k, qlm::Border::BORDER_REFLECT);
	t.end();
	
	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
}
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 22 ms

# Filter 2D

## Description
Applys 2D filter of size MxN on the input image
## C++ API
```c++
namespace qlm
{
	sf::Image Filter2D(
		const sf::Image& in,
		const Kernel& kernel,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
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
| `in`           | `sf::Image`  | The input image.                                                                             |
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
	qlm::Timer<msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	qlm::Kernel k{ 3, 3 };
	// sharpen filter
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);

	// do the operation
	t.start();
	sf::Image out = qlm::Filter2D(in, k, qlm::BORDER::BORDER_REFLECT);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");
}
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 37 ms


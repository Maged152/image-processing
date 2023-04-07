# Separable 2D Filter 

## Description
Applies a separable linear filter of size MxN to the input image
## C++ API
```c++
namespace qlm
{
	sf::Image SepFilter2D(
		const sf::Image& in,
		const Kernel1D& x_kernel,
		const Kernel1D& y_kernel,
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

| Name           | Type         | Description                                                                       |
|----------------|--------------|-----------------------------------------------------------------------------------|
| `in`           | `sf::Image`  | The input image.                                                                  |
| `x_kernel`     | `Kernel1D`   | The kernel for filtering each row.                                                |
| `y_kernel`     | `Kernel1D`   | The kernel  for filtering each column.                                            |
| `border_type`  | `BORDER`     | The pixel extrapolation method.                                                   |
| `border_value` | `int`        | The value to be used if the border is BORDER::BORDER_CONSTANT.                    |

## Example 
	x_kernel = |1/3  1/3  1/3|
	
	y_kernel = | 1/3 |
	           | 1/3 |
               | 1/3 |


	border = BORDER::BORDER_REFLECT

```c++
    qlm::Timer<msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}

	qlm::Kernel1D ker{ 3 };
	ker.Set(0, 1.0/3); ker.Set(1, 1.0 / 3); ker.Set(2, 1.0 / 3);

	// do the operation
	t.start();
	sf::Image out = qlm::SepFilter2D(in, ker, ker, qlm::BORDER::BORDER_REFLECT);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 25 ms

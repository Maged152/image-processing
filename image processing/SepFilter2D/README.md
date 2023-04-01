# Separable 2D Filter 

## Description
Applies a separable linear filter of size MxN to the input image
## C++ API
```c++
namespace qlm
{
	void SepFilter2D(
		const sf::Image& in,
		      sf::Image& out,
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
| `out`          | `sf::Image`  | The output image.                                                                 |
| `x_kernel`     | `Kernel1D`   | The kernel for filtering each row.                                                |
| `y_kernel`     | `Kernel1D`   | The kernel  for filtering each column.                                            |
| `border_type`  | `BORDER`     | The pixel extrapolation method.                                                   |
| `border_value` | `int`        | The value to be used if the border is BORDER::BORDER_CONSTANT.                    |

## Result  for this  kernel 
	x_kernel = |1/3  1/3  1/3|
	
	y_kernel = | 1/3 |
	           | 1/3 |
               | 1/3 |


	border = BORDER::BORDER_REFLECT

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 165 ms

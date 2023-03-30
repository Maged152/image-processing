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
		const Kernel1D& kernel_x,
		const Kernel1D& kernel_y,
		      BORDER border = BORDER::BORDER_CONSTANT,
		      int border_const = 0
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
| `kernel_x`     | `Kernel1D`   | The kernel for filtering each row.                                                |
| `kernel_y`     | `Kernel1D`   | The kernel  for filtering each column.                                            |
| `border`       | `BORDER`     | The pixel extrapolation method.                                                   |
| `border_const` | `int`        | The value to be used if the border is BORDER::BORDER_CONSTANT.                    |


# Filter 2D

## Description
Applys 2D filter of size MxN on the input image
## C++ API
```c++
namespace qlm
{
	void Filter2D(
		const sf::Image& in,
		      sf::Image& out,
		const Kernel& kernel,
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

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `in`           | `sf::Image`  | The input image.                                                                             |
| `out`          | `sf::Image`  | The output image.                                                                             |
| `kernel`       | `Kernel`     | The kernel to be applied on the input image.                                                 |
| `border`       | `BORDER`     | The pixel extrapolation method.                                                              |
| `border_const` | `int`        | The value to be used if the border is BORDER::BORDER_CONSTANT.                               |


## Result  for this  kernel 
	|0  -1  0|  
	|-1  8 -1|
	|0  -1  0|

	border = :BORDER::BORDER_REFLECT

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 300 ms


# Scale

## Description
The function scales an image up or dowm

## C++ API
```c++
namespace qlm
{
	void Scale(
		    const sf::Image& in,
			  sf::Image& out,
			  ScaleMethod method,
			  float scale_x,
			  float scale_y )
}
```
```c++
namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	}
}
```
## Parameters

| Name      | Type         | Description                      |
|-----------|--------------|----------------------------------|
| `in`      | `sf::Image`  | The input image.                 |
| `out`     | `sf::Image`  | The output image.                |
| `method`  | `ScaleMethod`| The scale method.                |
| `scale_x` | `float`      | The scale factor in x direction. |
| `scale_y` | `float`      | The scale factor in x direction. |

## Results from scale by 2.7
### The input
![Input Image](input.jpg)
### Nearest neighbor
![Input Image](nearest_neighbor.jpg)

Time = 117 ms
### Bilinear
![Input Image](bilinear.jpg)

Time = 311 ms
### Bicubic
![Input Image](bicubic.jpg)

Time = 1180 ms

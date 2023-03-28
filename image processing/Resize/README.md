# Resize

## Description
The function resizes the input image to the new given dimensions

## C++ API
```c++
namespace qlm
{
	void Resize(
		const sf::Image& in,
		      sf::Image& out,
		const unsigned int width,
		const unsigned int height,
		ScaleMethod method = ScaleMethod::BILINEAR
	);
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

| Name    | Type           | Description                      |
|---------|----------------|----------------------------------|
| `in`    | `sf::Image`    | The input image.                 |
| `out`   | `sf::Image`    | The output image.                |
| `width` | `unsigned int` | The new width.                   |
| `height`| `unsigned int` | The new height.                  |
| `method`| `ScaleMethod`  | The scale method.                |

## Results for these parameters 
    width = 960 
	height = 540
	method = ScaleMethod::BILINEAR
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 380 ms

# Flood Fill

## Description
The function performs region filling starting from certain point with a given color.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> FloodFill(
		const Image<frmt, T>& in,
		const Point<int>& seed_point,
		const Pixel<frmt, T>& fill_color,
		const Pixel<frmt, T>& threshold = Pixel<frmt, T>{});
}
```

## Parameters

| Name         | Type         | Description                                                               |
|--------------|--------------|---------------------------------------------------------------------------|
| `in`         | `Image`      | The input image.                                                          |
| `seed_point` | `Point<int>` | seed point location.                                                      |
| `fill_color` | `Pixel`      | the color to fill the area.                                               |
| `threshold`  | `Pixel`      | the threshold which determines if the point brlongs to the region or not. |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Color%20Manipulation/FloodFill)
* [Implementation](../../../../code/FloodFill/FloodFill.cpp)

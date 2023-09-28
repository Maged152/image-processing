# Resize

## Description
The function resizes the input image to the new given dimensions

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Resize(
		const Image<frmt, T>& in,
		const unsigned int width,
		const unsigned int height,
		const InterpolationFlag method = InterpolationFlag::BILINEAR,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name           | Type           | Description                      |
|----------------|----------------|----------------------------------|
| `in`           | `Image`        | The input image.                 |
| `width`        | `unsigned int` | The new width.                   |
| `height`       | `unsigned int` | The new height.                  |
| `method`       | `ScaleMethod`  | The scale method.                |
| `border_mode`  | `BorderMode`   | The pixel extrapolation method.  |

* [Example](../../../Examples/Geometric%20Transformations/Resize)
* [Implementation](../../../../code/Resize/Resize.cpp)
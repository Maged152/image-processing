# Scale

## Description
The function scales an image up or down

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Scale(const Image<frmt, T>& in,
						 const InterpolationFlag method,
		                 const float scale_x, 
		                 const float scale_y,
						 const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name           | Type               | Description                      |
|----------------|--------------------|----------------------------------|
| `in`           | `Image`            | The input image.                 |
| `method`       | `InterpolationFlag`| The scale method.                |
| `scale_x`      | `float`            | The scale factor in x direction. |
| `scale_y`      | `float`            | The scale factor in x direction. |
| `border_mode`  | `BorderMode`       | The pixel extrapolation method.  |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/Scale)
* [Implementation](../../../../code/Scale/Scale.cpp)
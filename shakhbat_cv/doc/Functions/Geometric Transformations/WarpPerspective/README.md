# Wrap Perspective

## Description
Applies a perspective transformation to an image.

The function warpPerspective transforms the source image using 3x3 transformation matrix:
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpPerspective(
		const Image<frmt, T>& in,
		const PerspectiveMatrix& mat,
		const int dst_width,
		const int dst_height,
		const InterpolationFlag inter,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name         | Type                  | Description                      |
|--------------|-----------------------|----------------------------------|
| `in`         | `Image`               | The input image.                 |
| `mat`        | `PerspectiveMatrix`   | 3×3 transformation matrix.       |
| `dst_width`  | `int`                 | Output image width.              |
| `dst_height` | `int`                 | Output image height.             |
| `inter`      | `InterpolationFlag`   | The interpolation method.        |
| `border_mode`| `BorderMode`          | The pixel extrapolation method.  |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/WarpPerspective)
* [Implementation](../../../../code/WarpPerspective/WarpPerspective.cpp)
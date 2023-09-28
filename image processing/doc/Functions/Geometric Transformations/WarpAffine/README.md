# Wrap Affine

## Description
Applies an affine transformation to an image.

The function WarpAffine transforms the input image using 2x3 transformation matrix:
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(
		const Image<frmt, T>& in,
		const TransformationMatrix& mat,
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
| `mat`        | `TransformationMatrix`| 2×3 transformation matrix.       |
| `dst_width`  | `int`                 | Output image width.              |
| `dst_height` | `int`                 | Output image height.             |
| `inter`      | `InterpolationFlag`   | The interpolation method.        |
| `border_mode`| `BorderMode`          | The pixel extrapolation method.  |


* [Example](../../../Examples/Geometric%20Transformations/WarpAffine)
* [Implementation](../../../../code/WarpAffine/WarpAffine.cpp)
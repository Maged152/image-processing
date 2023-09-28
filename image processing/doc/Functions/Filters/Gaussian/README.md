# Gaussian

## Description
Applys gaussian filter of size NxN on the input image.

## C++ API


```c++
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Gaussian(
		const Image<frmt, T>& in,
		const unsigned int filter_size,
		float sigma_x,
		float sigma_y = 0,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	)
```
## Parameters

| Name           | Type           | Description                                                                                              |
|----------------|----------------|----------------------------------------------------------------------------------------------------------|
| `in`           | `Image`        | The input image.                                                                                         |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                                           |
| `sigma_x`      | `float`        | Gaussian kernel standard deviation in X direction.                                                       |
| `sigma_y`      | `float`        | Gaussian kernel standard deviation in Y direction, if sigmaY is zero, it is set to be equal to sigmaX    |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


* [Example](../../../Examples/Filters/Gaussian)
* [Implementation](../../../../code/Gaussian/Gaussian.cpp)
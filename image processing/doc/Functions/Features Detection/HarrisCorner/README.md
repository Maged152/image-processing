# Harris Corner

## Description
Harris corner detector
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T = uint8_t>
	std::vector<KeyPoint<int>> HarrisCorner(
		const Image<frmt, T>& in,
		const unsigned int block_size,
		const unsigned int filter_size,
		float k,
		float threshold,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	)
}
```

## Parameters

| Name           | Type          | Description                                                                                  |
|----------------|---------------|----------------------------------------------------------------------------------------------|
| `in`           | `Image`       | The input image.                                                                             |
| `block_size`   | `unsigned int`| The neighborhood size.                                                                       |
| `filter_size`  | `unsigned int`| The aperture parameter for the Sobel operator.                                               |
| `k`            | `float`       | Harris detector free parameter.															   |
| `threshold`    | `float`       | The threshold parameter. Only those corners are returned that have response ( >threshold ).  |
| `border_mode`  | `BorderMode`  | The pixel extrapolation method.                                                              |

## Return Value
The function returns a vector of key-points(corners) of type `std::vector<KeyPoint<int>>`.

* [Example](../../../Examples/Features%20Detection/HarrisCorner)
* [Implementation](../../../../code/HarrisCorner/HarrisCorner.cpp)

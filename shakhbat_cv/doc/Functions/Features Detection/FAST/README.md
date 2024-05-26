# FAST

## Description
Detects corners using the FAST algorithm

## C++ API
```c++
namespace qlm
{
	template<pixel_t T>
	std::vector<KeyPoint<int>> FAST(
		const Image<ImageFormat::GRAY, T>& in,
		const unsigned int arc_length = 9,
		const T threshold = 10,
		const bool non_max_suppression = true);
}
```

## Parameters

| Name                  | Type          | Description                                                                                               |
|-----------------------|---------------|-----------------------------------------------------------------------------------------------------------|
| `in`                  | `Image`       | The input image.                                                                                          |
| `arc_length`          | `unsigned int`| Arc length in pixels over the circle to check central pixel is a corner.                                  |
| `threshold`           | `pixel_t`     | threshold on difference between intensity of the central pixel and pixels of a circle around this pixel.  |
| `non_max_suppression` | `bool`        | if true, non-maximum suppression is applied to detected corners										    |


## Return Value
The function returns a vector of key-points(corners) of type `std::vector<KeyPoint<int>>`.

* [Example](../../../Examples/Features%20Detection/FAST)
* You can check the implementation [here](../../../../source/FAST.cpp)

# MinMax Location

## Description
The MinMaxLoc function finds the minimum and maximum intensity 
values within an image and their corresponding locations (coordinates).

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc(
		const Image<frmt, T>& in
	);
}
```

## Parameters

| Name                  | Type          | Description                                                                                               |
|-----------------------|---------------|-----------------------------------------------------------------------------------------------------------|
| `in`                  | `Image`       | The input image.                                                                                          |


## Return Value
The function returns a `std::pair<std::vector<Point<int>>, std::vector<Point<int>>>`.

 The first element of the pair is a std::vector<Point<int>> containing 
	  the locations (coordinates) of the minimum intensity values.

 The second element is a std::vector<Point<int>> containing 
		the locations (coordinates) of the maximum intensity values.


* [Example](../../../Examples/Pixel-Wise/MinMaxLoc)
* You can check the implementation [here](../../../../source/MinMaxLoc.cpp)

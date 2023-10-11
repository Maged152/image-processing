# Hough Lines

## Description
Finds lines in a binary image using the standard Hough transform
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::vector<LinePolar> HoughLines(const Image<frmt, T>& in,
									  float  rho, 
									  float theta_step, 
									  int threshold,
									  double min_theta = 0,
									  double max_theta = std::numbers::pi);
}
```


## Parameters

| Name        | Type         | Description                                                                                  |
|-------------|--------------|----------------------------------------------------------------------------------------------|
| `in`        | `Image`      | The input image.                                                                             |
| `rho`       | `float`      | The Distance resolution of the accumulator in pixels.                                        |
| `theta`     | `float`      | The angle resolution of the accumulator in radians.                                          |
| `threshold` | `int`        | The threshold parameter. Only those lines are returned that get enough votes ( >threshold ). |
| `min_theta` | `double`     | The minimum angle to check for lines. Must fall between 0 and max_theta.                     |
| `max_theta` | `double`     | The upper bound for the angle. Must fall between min_theta and PI.                         |

## Return Value
The function returns a vector of lines of type `std::vector<LinePolar>`.

* [Example](../../../Examples/Features%20Detection/HoughLines)
* [Implementation](../../../../code/HoughLines/HoughLines.cpp)

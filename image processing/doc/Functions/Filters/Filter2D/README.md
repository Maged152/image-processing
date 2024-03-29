# Filter 2D

## Description
Applys 2D filter of size MxN on the input image
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		const  BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);
}
```

## Parameters

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                             |
| `kernel`       | `Kernel`     | The kernel to be applied on the input image.                                                 |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Filters/Filter2D)
* [Implementation](../../../../code/Filter2D.cpp)

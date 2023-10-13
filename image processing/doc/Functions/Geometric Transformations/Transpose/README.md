# Transpose

## Description
The function transposes the input image.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Transpose(
		const Image<frmt, T>& in);
}
```


## Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| `in`        | `Image`    | The input image.                                                  |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/Transpose)
* [Implementation](../../../../code/Transpose/Transpose.cpp)


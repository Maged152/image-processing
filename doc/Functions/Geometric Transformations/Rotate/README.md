# Rotate

## Description
The function rotates an image  in multiples of 90 degrees.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Rotate(
		const Image<frmt, T>& in,
		const RotateFlag rotate_code);
}
```

## Parameters

| Name          | Type         | Description                                                         |
|---------------|--------------|---------------------------------------------------------------------|
| `in`          | `Image`      | The input image.                                                    |
| `rotate_code` | `RotateFlag` | An enum to specify how to rotate the image.                         |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/Rotate)
* [Implementation](../../../../code/Rotate.cpp)
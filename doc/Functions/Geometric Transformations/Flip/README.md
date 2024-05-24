# Flip

## Description
The function flips an image around a given axis.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Flip(
		const Image<frmt, T>& in,
		const FlipFlag flip_code);
}
```

## Parameters

| Name        | Type       | Description                                                       |
|-------------|------------|-------------------------------------------------------------------|
| `in`        | `Image`    | The input image.                                                  |
| `flip_code` | `FlipFlag` | An enum to specify how to flip the image.                         |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/Flip)
* [Implementation](../../../../code/Flip.cpp)

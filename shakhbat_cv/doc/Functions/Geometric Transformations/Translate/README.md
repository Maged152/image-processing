# Translate

## Description
The function Translates an image by a given displacement.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Translate(
		const Image<frmt, T>& in,
		const Point<int>& displacement,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});
}
```

## Parameters

| Name          | Type         | Description                                                         |
|---------------|--------------|---------------------------------------------------------------------|
| `in`          | `Image`      | The input image.                                                    |
| `displacement`| `Point`      | displacement                                                        |
| `pix`         | `Pixel`      | The pixel used to fill the background                               |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/Translate)
* You can check the implementation [here](../../../../source/Translate.cpp)
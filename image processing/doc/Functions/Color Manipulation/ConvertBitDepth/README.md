# Convert Bit Depth

## Description
The function converts an input image/pixel bit depth from `src_t` to `dst_t`. 

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Pixel<frmt, dst_t> ConvertBitDepth(const Pixel<frmt, src_t>& in);

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> ConvertBitDepth(const Image<frmt, src_t>& in);
}
```

## Parameters

| Name       | Type           | Description                      |
|------------|----------------|----------------------------------|
| `src_t`    | `pixel_t`      | The input image/pixel type.      |
| `dst_t`    | `pixel_t`      | The output image/pixel type.     |
| `in`       | `Image`        | The input image/pixel.           |

## Return Value
The function returns an image of type `Image<dst_frmt, dst_t>`.
The function returns an image of type `Pixel<dst_frmt, dst_t>`.

* [Example](../../../Examples/Color%20Manipulation/ConvertBitDepth)
* [Implementation](../../../../code/ConvertBitDepth/ConvertBitDepth.cpp)

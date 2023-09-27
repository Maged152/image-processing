# Color Convert

## Description
The function converts an input image of format `src_frmt` with pixel type `src_t` to 
an output image of format `dst_frmt` with pixel type `dst_t`.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat src_frmt, pixel_t src_t, ImageFormat dst_frmt, pixel_t dst_t>
	Image<dst_frmt, dst_t> ColorConvert(const Image<src_frmt, src_t>& in);
}
```

## Supported Conversions

- GRAY to RGB
- RGB to GRAY
- YCrCb to RGB
- RGB to YCrCb
- HSV to RGB
- RGB to HSV
- HLS to RGB
- RGB to HSL

## Parameters

| Name       | Type           | Description                      |
|------------|----------------|----------------------------------|
| `src_frmt` | `ImageFormat`  | The input image format.          |
| `src_t`    | `pixel_t`      | The input image type.            |
| `dst_frmt` | `ImageFormat`  | The output image format.         |
| `dst_t`    | `pixel_t`      | The output image type.           |
| `in`       | `Image`        | The input image.                 |


* [Example](../../../Examples/ColorConvert)
* [Implementation](../../../../code/ColorConvert/ColorConvert.cpp)

# Separable 2D Filter 

## Description
Applies a separable linear filter of size MxN to the input image
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> SepFilter2D(
		const Image<frmt, src_t>& in,
		const SepKernel& kernel,
		const BorderMode<frmt, src_t>& border_mode = BorderMode{}
	);
}
```

## Parameters

| Name           | Type         | Description                                                                       |
|----------------|--------------|-----------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                  |
| `kernel`       | `SepKernel`  | The kernel for filtering.                                                |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


* [Example](../../../Examples/Filters/SepFilter2D)
* [Implementation](../../../../code/SepFilter2D/SepFilter2D.cpp)

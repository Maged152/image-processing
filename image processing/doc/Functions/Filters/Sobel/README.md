# Sobel

## Description
Applys sobel filter of size NxN on the input image.

## C++ API

### `SobelX` Function
 Performs Sobel X derivative on the input image. input image must be in GRAY format

```c++
    template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelX(
		const Image <ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
```
## Parameters

| Name           | Type           | Description                                                                                  |
|----------------|----------------|----------------------------------------------------------------------------------------------|
| `in_t`         | `pixel_t`      | The data type of the input image.                                                            |
| `out_t`        | `pixel_t`      | The data type of the output image.                                                           |
| `in`           | `Image`        | The input image<GRAY, in_t>.                                                                 |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                               |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


### `SobelY` Function
 Performs Sobel Y derivative on the input image. input image must be in GRAY format

```c++
    template<pixel_t in_t, pixel_t out_t = int16_t>
	Image<ImageFormat::GRAY, out_t> SobelY(
		const Image<ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
```
## Parameters

| Name           | Type           | Description                                                                                  |
|----------------|----------------|----------------------------------------------------------------------------------------------|
| `in_t`         | `pixel_t`      | The data type of the input image.                                                            |
| `out_t`        | `pixel_t`      | The data type of the output image.                                                           |
| `in`           | `Image`        | The input image<GRAY, in_t>.                                                                 |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                               |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


### `Sobel` Function
 Performs Sobel derivative on the input image. input image must be in GRAY format
 The return data type is `SobelDerivatives`

```c++
   template<pixel_t in_t, pixel_t out_t = int16_t>
	SobelDerivatives<in_t, out_t> Sobel(
		const Image<ImageFormat::GRAY, in_t>& in,
		const unsigned int kernel_size,
		const BorderMode<ImageFormat::GRAY, in_t>& border_mode = BorderMode<ImageFormat::GRAY, in_t>{}
	);
```
## Parameters

| Name           | Type           | Description                                                                                  |
|----------------|----------------|----------------------------------------------------------------------------------------------|
| `in_t`         | `pixel_t`      | The data type of the input image.                                                            |
| `out_t`        | `pixel_t`      | The data type of the output image.                                                           |
| `in`           | `Image`        | The input image<GRAY, in_t>.                                                                 |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                               |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


### `ConvertSobelDepth` Function
 Change Bit Depth of sobel from S16 to U8

```c++
  Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(Image < ImageFormat::GRAY, int16_t>& in,
	                                                    unsigned int filter_size);
```
## Parameters

| Name           | Type           | Description                                 |
|----------------|----------------|---------------------------------------------| 
| `in`           | `Image`        | The input image<GRAY, uint8_t>.             |
| `filter_size`  | `unsigned int` | The filter size used for the image.         |


* [Example](../../../Examples/Sobel)
* [Implementation](../../../../code/Sobel/Sobel.cpp)

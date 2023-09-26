# Wrap Affine

## Description
Applies an affine transformation to an image.

The function WarpAffine transforms the input image using 2x3 transformation matrix:
## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WarpAffine(
		const Image<frmt, T>& in,
		const TransformationMatrix& mat,
		const int dst_width,
		const int dst_height,
		const InterpolationFlag inter,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name         | Type                  | Description                      |
|--------------|-----------------------|----------------------------------|
| `in`         | `Image`               | The input image.                 |
| `mat`        | `TransformationMatrix`| 2×3 transformation matrix.       |
| `dst_width`  | `int`                 | Output image width.              |
| `dst_height` | `int`                 | Output image height.             |
| `inter`      | `InterpolationFlag`   | The interpolation method.        |
| `border_mode`| `BorderMode`          | The pixel extrapolation method.  |


### Get Affine transformation

```c++
    TransformationMatrix GetRotationMatrix(const Point<int>& center, const float angle, const float scale);

	TransformationMatrix GetAffineTransform(const Point<int> src[3], const Point<int> dst[3]);
```
## Example

```c++
	qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;


	float scale = 0.5f;
	float angle = 30.0f;
	const int dst_width = in.Width();
	const int dst_height = in.Height();

	const qlm::Point<int> center{ (int)in.Width() / 2, (int)in.Height() / 2 };

	qlm::TransformationMatrix mat = qlm::GetRotationMatrix(center, angle, scale);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;
	
	// do the operation
	t.start();
	auto out = qlm::WarpAffine(in, mat, dst_width, dst_height, qlm::InterpolationFlag::BILINEAR, border_mode);
	t.end();

	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 3 ms


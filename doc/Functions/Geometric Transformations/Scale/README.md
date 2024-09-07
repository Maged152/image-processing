# Scale

## Description
The function scales an image up or down

You can check the implementation [here](../../../../source/Scale.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Scale(const Image<frmt, T>& in,
						 const InterpolationFlag method,
		                 const float scale_x, 
		                 const float scale_y,
						 const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{});
}
```

## Parameters

| Name           | Type               | Description                      |
|----------------|--------------------|----------------------------------|
| `in`           | `Image`            | The input image.                 |
| `method`       | `InterpolationFlag`| The scale method.                |
| `scale_x`      | `float`            | The scale factor in x direction. |
| `scale_y`      | `float`            | The scale factor in x direction. |
| `border_mode`  | `BorderMode`       | The pixel extrapolation method.  |

## Return Value
The function returns an image of type `Image<frmt, T>`.

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

	float scale_x = 2.7f;
	float scale_y = 2.7f;

	// do the operation
	t.Start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_nn = qlm::Scale(in, qlm::InterpolationFlag::NEAREST_NEIGHBOR, scale_x, scale_y);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";

	if (!out_nn.SaveToFile("nearest_neighbor.jpg", alpha))
	{
		std::cout << "Failed to write nearest_neighbor\n";
	}

	// do the operation
	t.Start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_bl = qlm::Scale(in, qlm::InterpolationFlag::BILINEAR, scale_x, scale_y);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";

	if (!out_bl.SaveToFile("bilinear.jpg", alpha))
	{
		std::cout << "Failed to write bilinear\n";
	}

	// do the operation
	t.Start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_bc = qlm::Scale(in, qlm::InterpolationFlag::BICUBIC, scale_x, scale_y);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";

	if (!out_bc.SaveToFile("bicubic.jpg", alpha))
	{
		std::cout << "Failed to write bicubic\n";
	}
```
### The input
![Input Image](input.jpg)
### Nearest neighbor
![Input Image](nearest_neighbor.jpg)

Time = 16 ms
### Bilinear
![Input Image](bilinear.jpg)

Time = 17 ms
### Bicubic
![Input Image](bicubic.jpg)

Time = 95 ms

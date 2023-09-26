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
		const Kernel1D& x_kernel,
		const Kernel1D& y_kernel,
		const BorderMode<frmt, src_t>& border_mode = BorderMode{}
	);
}
```
```c++
namespace qlm
{
	enum class BORDER
	{
		BORDER_CONSTANT,
		BORDER_REPLICATE,
		BORDER_REFLECT,
	};

	template<ImageFormat frmt, pixel_t T>
	struct BorderMode
	{
		BorderType border_type = BorderType::BORDER_CONSTANT;
		Pixel<frmt, T> border_pixel{};
	};
}
```
## Parameters

| Name           | Type         | Description                                                                       |
|----------------|--------------|-----------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                  |
| `x_kernel`     | `Kernel1D`   | The kernel for filtering each row.                                                |
| `y_kernel`     | `Kernel1D`   | The kernel  for filtering each column.                                            |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |

## Example 
	x_kernel = |1/3  1/3  1/3|
	
	y_kernel = | 1/3 |
	           | 1/3 |
               | 1/3 |


	border = BORDER::BORDER_REFLECT

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

	qlm::Kernel1D ker{ 3 };
	ker.Set(0, 1.0 / 3); ker.Set(1, 1.0 / 3); ker.Set(2, 1.0 / 3);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, ker, ker, border_mode);
	t.end();

	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}

```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 12 ms

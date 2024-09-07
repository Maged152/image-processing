# Gaussian

## Description
Apples gaussian filter of size NxN on the input image.

You can check the implementation [here](../../../../source/Gaussian.cpp)

## C++ API


```c++
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Gaussian(
		const Image<frmt, T>& in,
		const unsigned int filter_size,
		float sigma_x,
		float sigma_y = 0,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	)
```
## Parameters

| Name           | Type           | Description                                                                                              |
|----------------|----------------|----------------------------------------------------------------------------------------------------------|
| `in`           | `Image`        | The input image.                                                                                         |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                                           |
| `sigma_x`      | `float`        | Gaussian kernel standard deviation in X direction.                                                       |
| `sigma_y`      | `float`        | Gaussian kernel standard deviation in Y direction, if sigmaY is zero, it is set to be equal to sigmaX    |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |

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

	unsigned int filter_size = 3;
	float sigma = 1.0f;
	// do the operation
	t.Start();
	auto out = qlm::Gaussian<qlm::ImageFormat::RGB, uint8_t>(in, filter_size, sigma);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 13 ms

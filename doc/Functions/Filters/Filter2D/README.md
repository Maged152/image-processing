# Filter 2D

## Description
Apples 2D filter of size MxN on the input image

You can check the implementation [here](../../../../source/Filter2D.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Filter2D(
		const Image<frmt, T>& in,
		const Kernel& kernel,
		const  BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	);
}
```

## Parameters

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                             |
| `kernel`       | `Kernel`     | The kernel to be applied on the input image.                                                 |
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

	qlm::Kernel k{ 3, 3 };
	// sharpen filter
	k.Set(0, 0, 0); k.Set(0, 1, -1); k.Set(0, 2, 0);
	k.Set(1, 0, -1); k.Set(1, 1, 5); k.Set(1, 2, -1);
	k.Set(2, 0, 0); k.Set(2, 1, -1); k.Set(2, 2, 0);

	// do the operation
	t.Start();
	auto out = qlm::Filter2D(in, k, qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{});
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

Time = 21 ms
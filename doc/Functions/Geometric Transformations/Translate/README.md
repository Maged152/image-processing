# Translate

## Description
The function Translates an image by a given displacement.

You can check the implementation [here](../../../../source/Translate.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T, typename S>
	Image<frmt, T> Translate(
		const Image<frmt, T>& in,
		const Point<S>& displacement,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});
}
```

## Parameters

| Name          | Type         | Description                                                         |
|---------------|--------------|---------------------------------------------------------------------|
| `in`          | `Image`      | The input image.                                                    |
| `displacement`| `Point<S>`   | displacement                                                        |
| `pix`         | `Pixel`      | The pixel used to fill the background                               |

The template type `S` controls how the image is sampled while translating:

| Type    | Description                                                          |
|---------|----------------------------------------------------------------------|
| `int`   | Uses nearest-neighbor sampling (`GetPixel`).                         |
| `float` | Uses bilinear interpolation for smoother sub-pixel displacement.     |

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

	qlm::Point<int> displacement{ 50 ,50 };
	// do the operation
	t.Start();
	auto out = qlm::Translate(in, displacement);
	t.End();

	std::cout <<"Time = " << t.ElapsedString() << "\n";


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 1 ms
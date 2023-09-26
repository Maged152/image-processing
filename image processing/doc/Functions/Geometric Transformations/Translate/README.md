# Translate

## Description
The function Translates an image by a given displacement.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> Translate(
		const Image<frmt, T>& in,
		const Point<int>& displacement,
		const Pixel<frmt, T>& pix = Pixel<frmt, T>{});
}
```
```c++
namespace qlm
{
	
	template<class T = int>
	class Point
	{
	public:
		T x;
		T y;
	public:
		Point() : x(0), y(0)
		{}
		Point(T x, T y) : x(x), y(y)
		{}
		// operatos +,==,=,- are supported
	}
}
```
## Parameters

| Name          | Type         | Description                                                         |
|---------------|--------------|---------------------------------------------------------------------|
| `in`          | `Image`      | The input image.                                                    |
| `displacement`| `Point`      | displacement                                                        |
| `pix`         | `Pixel`      | The pixel used to fill the background                               |

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

	qlm::Point displacement{ 50 ,50 };
	// do the operation
	t.start();
	auto out = qlm::Translate(in, displacement);
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

Time = 1 ms


# Scale

## Description
The function rotates an image for an angle around a centre by the transformation matrix.

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> WrapRotate(
		const Image<frmt, T>& in,
		      float angle,
		const Point<int>& centre,
		const float scale = 1,
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

| Name      | Type         | Description                                                         |
|-----------|--------------|---------------------------------------------------------------------|
| `in`      | `Image`      | The input image.                                                    |
| `angle`   | `float`      | Rotation angle in degrees. Positive values mean clockwise rotation. |
| `centre`  | `Point`      | Center of the rotation in the source image.                         |
| `scale`   | `float`      | The scale factor.                                                   |
| `pix`     | `Pixel`      | The pixel used to fill the background                               |

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

	float angle = 30.0f;
	float scale = 1.0f;
	int centre_x = (in.Width() - 1) / 2;
	int centre_y = (in.Height() - 1) / 2;
	qlm::Point<int> centre{ centre_x, centre_y };
	// background color
	qlm::Pixel<qlm::ImageFormat::RGB, uint8_t> pix{ 255, 0, 0 };
	// do the operation
	t.start();
	auto out = qlm::WrapRotate(in, angle, centre, scale, pix);
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


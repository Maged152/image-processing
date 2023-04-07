# Scale

## Description
The function scales an image up or dowm

## C++ API
```c++
namespace qlm
{
	sf::Image Scale(
			const sf::Image& in,
			      ScaleMethod method,
			      float scale_x,
			      float scale_y )
}
```
```c++
namespace qlm
{
	enum class ScaleMethod
	{
		NEAREST_NEIGHBOR,
		BILINEAR,
		BICUBIC
	}
}
```
## Parameters

| Name      | Type         | Description                      |
|-----------|--------------|----------------------------------|
| `in`      | `sf::Image`  | The input image.                 |
| `method`  | `ScaleMethod`| The scale method.                |
| `scale_x` | `float`      | The scale factor in x direction. |
| `scale_y` | `float`      | The scale factor in x direction. |

## Example

```c++
	qlm::Timer<msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	sf::Image in;
	if (!in.loadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	float scale_x = 2.7f;
	float scale_y = 2.7f;

	// do the operation
	t.start();
	sf::Image out_nn = qlm::Scale(in, qlm::ScaleMethod::NEAREST_NEIGHBOR, scale_x, scale_y);
	t.end();

	t.show();
	// Save the image to a file
	out_nn.saveToFile("nearest_neighbor.jpg");

	t.start();
	sf::Image out_bl = qlm::Scale(in, qlm::ScaleMethod::BILINEAR, scale_x, scale_y);
	t.end();

	t.show();
	// Save the image to a file
	out_bl.saveToFile("bilinear.jpg");

	t.start();
	sf::Image out_bc = qlm::Scale(in, qlm::ScaleMethod::BICUBIC, scale_x, scale_y);
	t.end();

	t.show();
	// Save the image to a file
	out_bc.saveToFile("bicubic.jpg");
```
### The input
![Input Image](input.jpg)
### Nearest neighbor
![Input Image](nearest_neighbor.jpg)

Time = 7 ms
### Bilinear
![Input Image](bilinear.jpg)

Time = 12 ms
### Bicubic
![Input Image](bicubic.jpg)

Time = 33 ms

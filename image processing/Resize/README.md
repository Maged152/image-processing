# Resize

## Description
The function resizes the input image to the new given dimensions

## C++ API
```c++
namespace qlm
{
	sf::Image Resize(
		const sf::Image& in,
		const unsigned int width,
		const unsigned int height,
		      ScaleMethod method = ScaleMethod::BILINEAR
	);
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

| Name    | Type           | Description                      |
|---------|----------------|----------------------------------|
| `in`    | `sf::Image`    | The input image.                 |
| `width` | `unsigned int` | The new width.                   |
| `height`| `unsigned int` | The new height.                  |
| `method`| `ScaleMethod`  | The scale method.                |

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
	unsigned int width = 960u;
	unsigned int height = 540u;

	// do the operation
	t.start();
	sf::Image out = qlm::Resize(in, width, height);
	t.end();

	t.show();
	// Save the image to a file
	out.saveToFile("result.jpg");
```
### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 20 ms

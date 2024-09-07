# Convert Bit Depth

## Description
The function converts an input image/pixel bit depth from `src_t` to `dst_t`. 

You can check the implementation [here](../../../../source/ConvertBitDepth.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Pixel<frmt, dst_t> ConvertBitDepth(const Pixel<frmt, src_t>& in);

	template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
	Image<frmt, dst_t> ConvertBitDepth(const Image<frmt, src_t>& in);
}
```

## Parameters

| Name       | Type           | Description                      |
|------------|----------------|----------------------------------|
| `src_t`    | `pixel_t`      | The input image/pixel type.      |
| `dst_t`    | `pixel_t`      | The output image/pixel type.     |
| `in`       | `Image`        | The input image/pixel.           |

## Return Value
The function returns an image of type `Image<dst_frmt, dst_t>`.
The function returns an image of type `Pixel<dst_frmt, dst_t>`.

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

	// U8 to S16
	t.Start();
	auto out = qlm::ConvertBitDepth<qlm::ImageFormat::RGB,uint8_t, int16_t>(in);
	t.End();
	
	std::cout <<"Time = " << t.ElapsedString() << "\n";
```


Time = 2 ms

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
- The Image overload returns an image of type `Image<frmt, dst_t>`.
- The Pixel overload returns a pixel of type `Pixel<frmt, dst_t>`.

## Notes

- Conversion is a full-range remap: the source range `[lowest, max]` is normalized to `[0, 1]` and then scaled to the destination range.
  Example: `uint8_t 0..255 -> int16_t -32768..32767`, so a u8→s16→u8 round trip is the identity.
- Values are clamped to `[0, 1]` after normalization and before denormalization (float rounding safety).
- Instantiated combos: GRAY/RGB `u8<->s16`, GRAY `u16->u8`, and the corresponding Pixel overloads.

## Example

```c++
   qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.Read(file_name))
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

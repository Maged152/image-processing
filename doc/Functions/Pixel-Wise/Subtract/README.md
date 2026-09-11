# Subtract

## Description
Computes the per-pixel difference `in1 - in2`. The subtraction is saturating: the result is clamped to the range of the output pixel type.

You can check the implementation [here](../../../../source/Subtract.cpp)

## C++ API
### In-place overload
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    void Subtract(
        const Image<frmt, T_in> &in1,
        const Image<frmt, T_in> &in2,
              Image<frmt, T_out> &out,
        const Rectangle<int> &roi = Rectangle<int>{});
}
```
### Allocating overload
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t T_in, pixel_t T_out>
    Image<frmt, T_out> Subtract(
        const Image<frmt, T_in> &in1,
        const Image<frmt, T_in> &in2,
        const Rectangle<int> &roi = Rectangle<int>{});
}
```

## Parameters

| Name  | Type              | Description                                                                  |
|-------|-------------------|------------------------------------------------------------------------------|
| `in1` | `Image`           | The first input image.                                                       |
| `in2` | `Image`           | The second input image.                                                      |
| `out` | `Image`           | The output image (in-place overload only). Same dimensions as the inputs.    |
| `roi` | `Rectangle<int>`  | Region of interest to process. Default `{}` processes the whole image.       |

## Return Value
The function returns an image of type `Image<frmt, T_out>` (allocating overload).

Note: currently only `Image<GRAY, uint8_t> -> Image<GRAY, int16_t>` is instantiated.

## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg";
    // load the image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in_rgb;
    if (!in_rgb.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }
    auto in = qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>(in_rgb);

    // subtract the image from itself -> zero image
    t.Start();
    auto out = qlm::Subtract<qlm::ImageFormat::GRAY, uint8_t, int16_t>(in, in);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";
```
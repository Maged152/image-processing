# Min

## Description
Finds the minimum pixel within an image (or a region of interest). For multi-channel images, the result is computed per-channel: each channel of the returned pixel holds the minimum value of that channel over the processed region.

You can check the implementation [here](../../../../source/Min.cpp)

## C++ API
### In-place overload
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    void Min(
        const Image<frmt, T>& in,
              Pixel<frmt, T>& out,
        const Rectangle<int>& roi = Rectangle<int>{});
}
```
### Allocating overload
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    Pixel<frmt, T> Min(
        const Image<frmt, T>& in,
        const Rectangle<int>& roi = Rectangle<int>{});
}
```

## Parameters

| Name  | Type               | Description                                                                        |
|-------|--------------------|------------------------------------------------------------------------------------|
| `in`  | `Image`            | The input image.                                                                   |
| `out` | `Pixel`            | The output pixel (in-place overload only). Holds the per-channel minimum.          |
| `roi` | `Rectangle<int>`   | Region of interest to process. Default `{}` processes the whole image.             |

## Return Value
The function returns a `Pixel<frmt, T>` containing the per-channel minimum value inside the ROI (allocating overload).

Note: currently `Image<GRAY, uint8_t>` and `Image<GRAY, float>` are instantiated.

## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg";
    // load the image
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
    if (!in.Read(file_name))
    {
        std::cout << "Failed to read the image\n";
        return -1;
    }

    t.Start();
    auto out = qlm::Min<qlm::ImageFormat::GRAY, uint8_t>(in);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";
    std::cout <<"min = " << out.v << "\n";
```

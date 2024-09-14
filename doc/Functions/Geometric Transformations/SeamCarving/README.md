# Seam Carving

## Description
Seam carving is an algorithm for content-aware image 
resizing [paper](https://dl.acm.org/doi/10.1145/1275808.1276390).

You can check the implementation [here](../../../../source/SeamCarving.cpp)

## C++ API
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> SeamCarving(
        const Image<frmt, T>& in,
        const size_t width,
        const size_t height,
        const EnergyFlag energy = EnergyFlag::BACKWARD,
        const OrderFlag order = OrderFlag::WIDTH_FIRST);
}
```

## Parameters

| Name     | Type         | Description                                            |
|----------|--------------|--------------------------------------------------------|
| `in`     | `Image`      | The input image.                                       |
| `width`  | `size_t`     | The desired width of the output image.                 |
| `height` | `size_t`     | The desired height of the output image.                |
| `energy` | `EnergyFlag` | The energy function to use (BACKWARD or FORWARD).      |
| `order`  | `OrderFlag`  | The order of resizing (WIDTH_FIRST or HEIGHT_FIRST).   |

## Return Value
The function returns an image of type `Image<frmt, T>`.

## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg"; 
    // load the input image
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

    const size_t target_width = in.width - 500;
    const size_t target_height =  in.height + 200;

    const qlm::EnergyFlag energy = qlm::EnergyFlag::BACKWARD;
    const qlm::OrderFlag order = qlm::OrderFlag::WIDTH_FIRST;

    t.Start();
    auto out = qlm::SeamCarving(in, target_width, target_height, energy, order);
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

Time = 125519 ms
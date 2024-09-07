# Box Filter 

## Description
Blurs an image using the box filter of size MxN.

The function smooths an image using the kernel:
```
             | 1 1 1 ... 1 1 |
             | 1 1 1 ... 1 1 |
filter = a * | 1 1 1 ... 1 1 |
             | ..... ... ... |
             | 1 1 1 ... 1 1 |

 
```
where a is `1 / (filter_x_size * filter_y_size)` if `normalize` is `true` otherwise `1`

You can check the implementation [here](../../../../source/BoxFilter.cpp)

## C++ API
```c++
namespace qlm
{
    template<ImageFormat frmt, pixel_t src_t, pixel_t dst_t>
    Image<frmt, dst_t> BoxFilter(
        const Image<frmt, src_t>& in,
        const unsigned int filter_x_size,
        const unsigned int filter_y_size,
        const bool normalize, 
        const BorderMode<frmt, src_t>& border_mode = BorderMode{}
    );
}
```

## Parameters

| Name           | Type           | Description                                                      |
|----------------|----------------|------------------------------------------------------------------|
| `in`           | `Image`        | The input image.                                                 |
| `filter_x_size`| `unsigned int` | The filter size in x direction.                                  |
| `filter_y_size`| `unsigned int` | The filter size in y direction.                                  |
| `normalize`    | `bool`         | Specifies whether the kernel is normalized by its area or not.   |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                    |


## Return Value
The function returns an image of type `Image<frmt, dst_t>`.

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

    const unsigned int filter_x_size = 5;
    const unsigned int filter_y_size = 5;

    const bool normalize = true;

    auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
    border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

    // do the operation
    t.Start();
    auto out = qlm::BoxFilter<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, filter_x_size, filter_y_size, normalize, border_mode);
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

Time = 22 ms

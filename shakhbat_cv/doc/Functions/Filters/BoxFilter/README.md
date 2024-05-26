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

* [Example](../../../Examples/Filters/BoxFilter)
* You can check the implementation [here](../../../../source/BoxFilter.cpp)

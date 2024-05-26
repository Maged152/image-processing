# Seam Carving

## Description
Seam carving is an algorithm for content-aware image 
resizing [paper](https://dl.acm.org/doi/10.1145/1275808.1276390).

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

| Name     | Type         | Description                                                         |
|----------|--------------|--------------------------------------------------------|
| `in`     | `Image`      | The input image.                                       |
| `width`  | `size_t`     | The desired width of the output image.                 |
| `height` | `size_t`     | The desired height of the output image.                |
| `energy` | `EnergyFlag` | The energy function to use (BACKWARD or FORWARD).      |
| `order`  | `OrderFlag`  | The order of resizing (WIDTH_FIRST or HEIGHT_FIRST).   |

## Return Value
The function returns an image of type `Image<frmt, T>`.

* [Example](../../../Examples/Geometric%20Transformations/SeamCarving)
* You can check the implementation [here](../../../../source/SeamCarving.cpp)
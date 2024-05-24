# Image

The `Image` class represents an image with a specified format and pixel type. It provides
various methods to manipulate and access pixel data within the image.

## Constructors


##	Image()
Creates an empty image with zero width and height.

## Parameters

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `in`           | `Image`      | The input image.                                                                             |
| `kernel`       | `Kernel`     | The kernel to be applied on the input image.                                                 |
| `border_mode`  | `BorderMode` | The pixel extrapolation method.                                                              |


## Return Value
The function returns an image of type `Image<frmt, T>`.
  
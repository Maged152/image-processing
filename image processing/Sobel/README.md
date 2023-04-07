# Sobel

## Description
Applys sobel filter of size NxN on the input image.

We provide three functions for performing Sobel edge detection on images, as well as a new data type to hold the output of the edge detection.

### ``SobelDerivatives`` Data Type
The  struct  contains four members:

| Name           | Type         | Description                                                                                  |
|----------------|--------------|----------------------------------------------------------------------------------------------|
| `sobel_x`      | `sf::Image`  | The Sobel X derivative of the input image.                                                   |
| `sobel_y`      | `sf::Image`  | The Sobel Y derivative of the input image.                                                   |
| `magnitude`    | `sf::Image`  | The magnitude of the Sobel derivatives.                                                      |
| `angle`        | `float*`     | A 2D array of float values that holds the angle of the Sobel derivatives.                    |
 
## C++ API

### `SobelX` Function
 Performs Sobel X derivative of the input image.

```c++
    sf::Image SobelX(
		const sf::Image& in,
		const unsigned int kernel_size,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);
```
### `Sobely` Function
Performs Sobel Y derivative of the input image.

```c++

	sf::Image SobelY(
		const sf::Image& in,
		const unsigned int kernel_size,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);
```

### ``Sobel`` Function
Performs both Sobel X & Sobel Y also calculate the magnitude and the angles

```c++
SobelDerivatives Sobel(
		const sf::Image& in,
		const unsigned int kernel_size,
		      BORDER border_type = BORDER::BORDER_CONSTANT,
		      int border_value = 0
	);
```


## Parameters

| Name           | Type           | Description                                                                                  |
|----------------|----------------|----------------------------------------------------------------------------------------------|
| `in`           | `sf::Image`    | The input image.                                                                             |
| `kernel_size`  | `unsigned int` | The kernel size to be used on the input image.                                               |
| `border_type`  | `BORDER`       | The pixel extrapolation method.                                                              |
| `border_value` | `int`          | The value to be used if the border is BORDER::BORDER_CONSTANT.                               |



# Get Kernel


## Get Gaussian coefficients

### Description
The `GetGaussianKernel` function is used to generate a one-dimensional Gaussian kernel with
a specified filter size and standard deviation (sigma). If `sigma` is not positive,
it is computed from the filter size. The kernel is normalized so its coefficients sum to 1.

### C++ API
```c++
	Kernel1D GetGaussianKernel(const int filter_size, float sigma);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'int'          |The size of the Gaussian kernel (odd integer).      |
| `sigma`       | 'float'        |The standard deviation of the Gaussian distribution.|

### Return Value
The function returns a one-dimensional Gaussian kernel of type `Kernel1D`.

* You can check the implementation [here](../../../../source/Gaussian.cpp)

## Get Derivative Kernel

### Description
The `GetDerivKernel` function is used to generate a one-dimensional derivative 
kernel with a specified size and order. It is built from the binomial (Pascal)
coefficients, which give a smoothing kernel for `order = 0` and a symmetric
derivative kernel for higher orders.

### C++ API
```c++
	Kernel1D GetDerivKernel(int filter_size, int order = 1);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'int'          |The size of the derivative kernel (odd integer).    |
| `order`       | 'int'          |The order of the derivative (default is 1).         |

### Return Value
The function returns a one-dimensional derivative kernel of type `Kernel1D`.

* You can check the implementation [here](../../../../source/Sobel.cpp)

## Get Derivative X Kernel

### Description
The `GetDerivXKernel` function is used to generate a sobel kernel with
a specified filter size in X direction: a derivative kernel of order 1
along X combined with a smoothing kernel (order 0) along Y.

### C++ API
```c++
	SepKernel GetDerivXKernel(int filter_size);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'int'          |The size of the sobel kernel (odd integer & >= 3).  |

### Return Value
The function returns a separable kernel of type `SepKernel`.

* You can check the implementation [here](../../../../source/Sobel.cpp)

## Get Derivative Y Kernel

### Description
The `GetDerivYKernel` function is used to generate a sobel kernel with
a specified filter size in Y direction: a smoothing kernel (order 0)
along X combined with a derivative kernel of order 1 along Y.

### C++ API
```c++
	SepKernel GetDerivYKernel(int filter_size);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'int'          |The size of the sobel kernel (odd integer & >= 3).  |

### Return Value
The function returns a separable kernel of type `SepKernel`.

* You can check the implementation [here](../../../../source/Sobel.cpp)

## Get Scharr X Kernel

### Description
The `GetScharrXKernel` function is used to generate the Scharr kernel in
X direction: a derivative kernel `[-1, 0, 1]` along X combined with a
smoothing kernel `[3, 10, 3]` along Y.

### C++ API
```c++
	SepKernel GetScharrXKernel();
```
### Return Value
The function returns a separable kernel of type `SepKernel` (3 x 3).

* You can check the implementation [here](../../../../source/Scharr.cpp)

## Get Scharr Y Kernel

### Description
The `GetScharrYKernel` function is used to generate the Scharr kernel in
Y direction: a smoothing kernel `[3, 10, 3]` along X combined with a
derivative kernel `[-1, 0, 1]` along Y.

### C++ API
```c++
	SepKernel GetScharrYKernel();
```
### Return Value
The function returns a separable kernel of type `SepKernel` (3 x 3).

* You can check the implementation [here](../../../../source/Scharr.cpp)

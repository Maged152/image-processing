# Get Kernel


## Get Gaussian coefficients

### Description
The `GetGaussianKernel` function is used to generate a one-dimensional Gaussian kernel with
a specified filter size and standard deviation (sigma).

### C++ API
```c++
	Kernel1D GetGaussianKernel(const unsigned int filter_size, float sigma);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'unsigned int' |The size of the Gaussian kernel (odd integer).      |
| `sigma`       | 'float'        |The standard deviation of the Gaussian distribution.|

### Return Value
The function returns a one-dimensional Gaussian kernel of type `Kernel1D`.

* [Implementation](../../../../code/Gaussian/Gaussian.cpp)

## Get Derivative Kernel

### Description
The `GetDerivKernel` function is used to generate a one-dimensional derivative 
kernel with a specified size and order.

### C++ API
```c++
	Kernel1D GetDerivKernel(unsigned int n, int order = 1);
```
### Parameters

| Name          | Type           | Description                                    |
| ------------- | ---------------|----------------------------------------------- |
| `filter_size` | 'unsigned int' | The size of the Gaussian kernel (odd integer). |
| `order`       | 'int'          | The order of the derivative (default is 1).    |

### Return Value
The function returns a one-dimensional derivative kernel of type `Kernel1D`.

* [Implementation](../../../../code/Sobel.cpp)

## Get Derivative X Kernel

### Description
The `GetDerivXKernel` function is used to generate a sobel kernel with
a specified filter size in X direction.

### C++ API
```c++
	SepKernel GetDerivXKernel(unsigned int filter_size);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'unsigned int' |The size of the Gaussian kernel (odd integer).      |

### Return Value
The function returns a separable kernel of type `SepKernel`.

* [Implementation](../../../../code/Sobel.cpp)

## Get Derivative Y Kernel

### Description
The `GetDerivYKernel` function is used to generate a sobel kernel with
a specified filter size in Y direction.

### C++ API
```c++
	SepKernel GetDerivYKernel(unsigned int filter_size);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'unsigned int' |The size of the Gaussian kernel (odd integer).      |

### Return Value
The function returns a separable kernel of type `SepKernel`.

* [Implementation](../../../../code/Sobel.cpp)

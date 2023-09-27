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


## Get Derivative X Kernel

### Description
The `GetDerivXKernel` function is used to generate a one-dimensional sobel kernel with
a specified filter size in X direction.

### C++ API
```c++
	SepKernel GetDerivXKernel(unsigned int n);
```
### Parameters

| Name          | Type           | Description                                        |
| ------------- | ---------------|--------------------------------------------------- |
| `filter_size` | 'unsigned int' |The size of the Gaussian kernel (odd integer).      |
| `sigma`       | 'float'        |The standard deviation of the Gaussian distribution.|

### Return Value
The function returns a one-dimensional Gaussian kernel of type `Kernel1D`.


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


## Get Derivative coefficients

```c++
    // generate derivative kernel
	Kernel1D GetDerivKernel(unsigned int n, int order = 1);
	// generate coefficients for Sobel X
	SepKernel GetDerivXKernel(unsigned int n);
	// generate coefficients for Sobel Y
	SepKernel GetDerivYKernel(unsigned int n);
	// change Bit Depth to sobel from S16 to U8
	Image<ImageFormat::GRAY, uint8_t> ConvertSobelDepth(Image < ImageFormat::GRAY, int16_t>& in,
	                                                    unsigned int filter_size);
```


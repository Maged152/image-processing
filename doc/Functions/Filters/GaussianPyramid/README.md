# Gaussian Pyramid

## Description
Constructs a Gaussian pyramid of the specified number of layers for a given input image, applying a Gaussian filter at each level and down sampling the image based on the provided scale factor.

You can check the implementation [here](../../../../source/GaussianPyramid.cpp)

## C++ API


```c++
	template<ImageFormat frmt, pixel_t T>
	Pyramid<frmt, T> GaussianPyramid(
		const Image<frmt, T>& in,
        const int num_layers,
		const float scale = 0.5f,
		const int filter_size = 5,
		const float sigma = 1.0f,
		const BorderMode<frmt, T>& border_mode = BorderMode<frmt, T>{}
	)
```
## Parameters

| Name          | Type           | Description                                                                                              |
|---------------|----------------|----------------------------------------------------------------------------------------------------------|
| `in`          | `Image`        | The input image.                                                                                         |
| `num_layers`  | `int`       | The number of layers to generate in the pyramid.                                                         |
| `scale`       | `float`        | The down sampling scale factor between consecutive layers (default is 0.5).                              |
| `filter_size` | `int`       | The kernel size for the Gaussian filter to be applied at each layer (default is 5)                       |
| `sigma`       | `float`        | The standard deviation for the Gaussian filter (default is 1.0).                                         |
| `border_mode` | `BorderMode`   | The pixel extrapolation method.                                                                          |

## Return Value
The function returns a pyramid of type `Pyramid<frmt, T>`.

## Example

```c++
     qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg"; 
    // load the input image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the input image\n";
        return -1;
    }

    const int num_layers = 5;

    t.Start();
    auto pyr = qlm::GaussianPyramid(in, num_layers);
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";

    // Save the output pyramid
    for (int layer = 0; layer < num_layers; layer++)
    {
        const std::string out_name = "layer_" + std::to_string(layer) + ".jpg";
        pyr.layers[layer].SaveToFile(out_name);
    }
```

### The input
![Input Image](input.jpg)
### The output
![Pyramid Image_0](layer_0.jpg)
![Pyramid Image_1](layer_1.jpg)
![Pyramid Image_2](layer_2.jpg)
![Pyramid Image_3](layer_3.jpg)
![Pyramid Image_4](layer_4.jpg)

Time = 694 ms

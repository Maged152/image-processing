# Multiply

## Description
The Multiply function performs element-wise multiplication of two images with an optional scale factor. 
It multiplies corresponding pixel values from two input images and applies overflow handling based on 
the specified flag (saturation or wrapping).

You can check the implementation [here](../../../../source/Multiply.cpp)

## C++ API
### In-place overload
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
	void Multiply(
		const Image<frmt, T1> &in1,
		const Image<frmt, T2> &in2,
              Image<frmt, TO> &out,
		const float scale = 1.0f,
		const OverFlowFlag &over_flow = OverFlowFlag::SATURATE,
		const Rectangle<int> &roi = Rectangle<int>{}
	);
}
```

### Allocating overload
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
	Image<frmt, TO> Multiply(
		const Image<frmt, T1> &in1,
		const Image<frmt, T2> &in2,
		const float scale = 1.0f,
		const OverFlowFlag &over_flow = OverFlowFlag::SATURATE,
		const Rectangle<int> &roi = Rectangle<int>{}
	);
}
```

## Parameters

| Name                  | Type             | Description                                                                                         |
|-----------------------|------------------|-----------------------------------------------------------------------------------------------------|
| `in1`                 | `Image`          | The first input image.                                                                              |
| `in2`                 | `Image`          | The second input image. Must have the same dimensions as `in1`.                                     |
| `out`                 | `Image`          | The output image (in-place overload only). Same dimensions as the inputs.                           |
| `scale`               | `float`          | Optional scale factor to apply to the multiplication result. Default is 1.0f.                       |
| `over_flow`           | `OverFlowFlag`   | Specifies overflow handling: SATURATE (clamp values) or WRAP (modular wrap). Default is SATURATE.   |
| `roi`                 | `Rectangle<int>` | Region of interest to process. Default `{}` processes the whole image.                              |

Note: the alpha channel is not processed — only `.v` (GRAY) or `.r/.g/.b` (RGB) are written; `.a` keeps its default value.
Pixels outside `roi` keep their default-constructed value in the allocating overload.


## Return Value
The function returns an `Image<frmt, TO>` containing the result of the element-wise multiplication.
The output image has the same dimensions as the input images.


## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name1 = "input1.jpg";
    std::string file_name2 = "input2.jpg";
    
    // load the input images
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in1;
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> in2;
    
    if (!in1.LoadFromFile(file_name1) || !in2.LoadFromFile(file_name2))
    {
        std::cout << "Failed to read the input images\n";
        return -1;
    }

    const float scale = 1.0f; 
  
    t.Start();
    auto out = qlm::Multiply<qlm::ImageFormat::RGB, uint8_t, uint8_t, uint8_t>(in1, in2, scale, qlm::OverFlowFlag::SATURATE);
    t.End();

    std::cout << "Time = " << t.ElapsedString() << "\n";
    
    // save the output image
    out.SaveToFile("output.jpg");
```

Note: this example uses its own `input1.jpg`/`input2.jpg` files (not shipped in this folder).

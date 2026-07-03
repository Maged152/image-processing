# Multiply

## Description
The Multiply function performs element-wise multiplication of two images with an optional scale factor. 
It multiplies corresponding pixel values from two input images and applies overflow handling based on 
the specified flag (saturation or wrapping).

You can check the implementation [here](../../../../source/Multiply.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T1, pixel_t T2, pixel_t TO>
	Image<frmt, TO> Multiply(
		const Image<frmt, T1> &in1,
		const Image<frmt, T2> &in2,
		const float scale = 1.0f,
		const OverFlowFlag &over_flow = OverFlowFlag::SATURATE
	);
}
```

## Parameters

| Name                  | Type          | Description                                                                                         |
|-----------------------|---------------|-----------------------------------------------------------------------------------------------------|
| `in1`                 | `Image`       | The first input image.                                                                              |
| `in2`                 | `Image`       | The second input image.                                                                             |
| `scale`               | `float`       | Optional scale factor to apply to the multiplication result. Default is 1.0f.                       |
| `over_flow`           | `OverFlowFlag`| Specifies overflow handling: SATURATE (clamp values) or WRAP (allow wrapping). Default is SATURATE. |


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

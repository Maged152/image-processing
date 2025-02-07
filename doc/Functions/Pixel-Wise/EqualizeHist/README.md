# EqualizeHist
## Description
The function applies Histogram Equalization on the input image.

Histogram equalization enhances the contrast of the image by redistributing the pixel intensity values.

You can check the implementation [here](../../../../source/EqualizeHist.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Image<frmt, T> EqualizeHist(const Image<frmt, T>& in)
}
```

## Parameters

| Name                  | Type                | Description                                                                                               |
|-----------------------|---------------------|-----------------------------------------------------------------------------------------------------------|
| `in`                  | `Image`             | The input image.                                                                                          |


## Return Value
The function returns an `Image<frmt, T>`, which is the result of the equalization operation applied to the input image


## Example
```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.png"; 
    // load the input image
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the input image\n";
        return -1;
    }

    t.Start();
    auto out = qlm::EqualizeHist(in);
    t.End();

	std::cout << "Histogram Equalization time = " << t.ElapsedString() << "\n";

    // Save the output image
    out.SaveToFile("result_histogram.jpg");
```

### The input
![Input Image](input.png)

### HISTOGRAM
![Input Image](result_histogram.jpg)

Time = 2 ms

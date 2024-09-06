# Histogram
## Description
The Histogram function calculates the histogram of an image. A histogram represents the distribution of pixel intensity values within the image.

You can check the implementation [here](../../../../source/Histogram.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	Histogram_t<frmt, T> Histogram(
		const Image<frmt, T>& in);
}
```

## Parameters

| Name                  | Type          | Description                                                                                               |
|-----------------------|---------------|-----------------------------------------------------------------------------------------------------------|
| `in`                  | `Image`       | The input image.                                                                                          |


## Return Value
The function returns a `Histogram_t<frmt, T>`, which represents the histogram of the input image. 


## Example
```c++
    qlm::Timer<qlm::msec> t{};
	std::string file_name = "input.jpg";
	// load the image
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> in;
	if (!in.LoadFromFile(file_name))
	{
		std::cout << "Failed to read the image\n";
		return -1;
	}
	// check alpha component
	bool alpha{ true };
	if (in.NumerOfChannels() == 3)
		alpha = false;

	t.start();
	auto out = qlm::Histogram(in);
	t.end();
	
	t.show();

    for (auto& channel : out.hist) 
    {
        std::cout << "[";
        for(int i = 0; i < out.tot_elements - 1; i++)
        {
            std::cout << channel[i] << ", ";
        }

        std::cout <<  channel[out.tot_elements - 1] << "]\n";
    }
```
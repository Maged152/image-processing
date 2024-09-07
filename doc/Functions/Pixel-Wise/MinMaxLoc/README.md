# MinMax Location

## Description
The MinMaxLoc function finds the minimum and maximum intensity 
values within an image and their corresponding locations (coordinates).

You can check the implementation [here](../../../../source/MinMaxLoc.cpp)

## C++ API
```c++
namespace qlm
{
	template<ImageFormat frmt, pixel_t T>
	std::pair<std::vector<Point<int>>, std::vector<Point<int>>> MinMaxLoc(
		const Image<frmt, T>& in
	);
}
```

## Parameters

| Name                  | Type          | Description                                                                                               |
|-----------------------|---------------|-----------------------------------------------------------------------------------------------------------|
| `in`                  | `Image`       | The input image.                                                                                          |


## Return Value
The function returns a `std::pair<std::vector<Point<int>>, std::vector<Point<int>>>`.

 The first element of the pair is a std::vector<Point<int>> containing 
	  the locations (coordinates) of the minimum intensity values.

 The second element is a std::vector<Point<int>> containing 
		the locations (coordinates) of the maximum intensity values.


## Example

```c++
    qlm::Timer<qlm::msec> t{};
    std::string file_name = "input.jpg";
    // load the input image
    qlm::Image<qlm::ImageFormat::GRAY, uint8_t> in;
    if (!in.LoadFromFile(file_name))
    {
        std::cout << "Failed to read the input image\n";
        return -1;
    }
  
    t.Start();
    auto [min_loc, max_loc] = qlm::MinMaxLoc(out);    
    t.End();

    std::cout <<"Time = " << t.ElapsedString() << "\n";

    std::cout << "number of min = " << min_loc.size() << "\n";
    std::cout << "number of max = " << max_loc.size() << "\n";
     
```
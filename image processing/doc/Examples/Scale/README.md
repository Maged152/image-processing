# Scale Example


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

	float scale_x = 2.7f;
	float scale_y = 2.7f;

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_nn = qlm::Scale(in, qlm::InterpolationFlag::NEAREST_NEIGHBOR, scale_x, scale_y);
	t.end();

	t.show();

	if (!out_nn.SaveToFile("nearest_neighbor.jpg", alpha))
	{
		std::cout << "Failed to write nearest_neighbor\n";
	}

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_bl = qlm::Scale(in, qlm::InterpolationFlag::BILINEAR, scale_x, scale_y);
	t.end();

	t.show();

	if (!out_bl.SaveToFile("bilinear.jpg", alpha))
	{
		std::cout << "Failed to write bilinear\n";
	}

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out_bc = qlm::Scale(in, qlm::InterpolationFlag::BICUBIC, scale_x, scale_y);
	t.end();

	t.show();

	if (!out_bc.SaveToFile("bicubic.jpg", alpha))
	{
		std::cout << "Failed to write bicubic\n";
	}
```
### The input
![Input Image](input.jpg)
### Nearest neighbor
![Input Image](nearest_neighbor.jpg)

Time = 16 ms
### Bilinear
![Input Image](bilinear.jpg)

Time = 17 ms
### Bicubic
![Input Image](bicubic.jpg)

Time = 95 ms

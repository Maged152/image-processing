# Gaussian Example

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

	unsigned int filter_size = 3;
	float sigma = 1.0f;
	// do the operation
	t.start();
	auto out = qlm::Gaussian<qlm::ImageFormat::RGB, uint8_t>(in, filter_size, sigma);
	t.end();

	t.show();


	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Falied to write \n";
	}
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 12 ms


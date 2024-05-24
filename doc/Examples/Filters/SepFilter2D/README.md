# Separable 2D Filter Example


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

	const int filter_size = 3;

	qlm::SepKernel box{ filter_size, filter_size };
	// set box filter coefficients
	for (int i = 0; i < filter_size; i++)
	{
		box.x_ker.Set(i, 1.0f / filter_size);
		box.y_ker.Set(i, 1.0f / filter_size);
	}

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// do the operation
	t.start();
	auto out = qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, box, border_mode);
	t.end();

	t.show();

	if (!out.SaveToFile("result.jpg", alpha))
	{
		std::cout << "Failed to write \n";
	}

```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 12 ms

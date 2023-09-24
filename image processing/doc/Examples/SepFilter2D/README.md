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

	qlm::Kernel1D ker{ 3 };
	ker.Set(0, 1.0 / 3); ker.Set(1, 1.0 / 3); ker.Set(2, 1.0 / 3);

	auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
	border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

	// do the operation
	t.start();
	qlm::Image<qlm::ImageFormat::RGB, uint8_t> out = qlm::SepFilter2D<qlm::ImageFormat::RGB, uint8_t, uint8_t>(in, ker, ker, border_mode);
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

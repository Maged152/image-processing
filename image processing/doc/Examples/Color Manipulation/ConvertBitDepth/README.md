# Convert Bit Depth Example


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

	// U8 to S16
	t.start();
	auto out = qlm::ConvertBitDepth<qlm::ImageFormat::RGB,uint8_t, int16_t>(in);
	t.end();
	
	t.show();
	}
```


Time = 2 ms

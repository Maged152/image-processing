# Box Filter Example


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

    const unsigned int filter_x_size = 5;
    const unsigned int filter_y_size = 5;

    const bool normalize = true;

    auto border_mode = qlm::BorderMode<qlm::ImageFormat::RGB, uint8_t>{};
    border_mode.border_type = qlm::BorderType::BORDER_REFLECT;

    // do the operation
    t.start();
    auto out = qlm::BoxFilter(in, filter_x_size, filter_y_size, normalize, border_mode);
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

Time = 22 ms

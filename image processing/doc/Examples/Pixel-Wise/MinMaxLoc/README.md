# MinMax Location Example


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
  
    t.start();
    auto [min_loc, max_loc] = qlm::MinMaxLoc(out);    
    t.show();

    t.end();

    std::cout << "nume of min = " << min_loc.size() << "\n";
    std::cout << "nume of max = " << max_loc.size() << "\n";
     
```


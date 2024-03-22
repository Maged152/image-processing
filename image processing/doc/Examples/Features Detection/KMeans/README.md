# KMeans Example


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
    if (in.NumerOfChannels() == 1)
        alpha = false;

    const unsigned int k = 4;
    const qlm::KMeansInit init = qlm::KMeansInit::KMEANS_PLUS_PLUS;

    // do the operation
    t.start();
    auto clusters = qlm::KMeans(in, k, init);
    t.end();

    t.show();

    // draw clusters
    auto out = qlm::DrawCluster(in, clusters);

    if (!out.SaveToFile("result.jpg", alpha))
    {
        std::cout << "Failed to write \n";
    }
```

### The input
![Input Image](input.jpg)
### The output
![Input Image](result.jpg)

Time = 3117 ms


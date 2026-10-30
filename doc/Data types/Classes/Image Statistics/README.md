# Image Statistics

Container types that hold the statistical outputs of the functions.

## Histogram_t&lt;frmt, T&gt;

A per-channel image histogram. Defined in [`include/Histogram.hpp`](../../../../include/Histogram.hpp).

```c++
template<ImageFormat frmt, typename T>
struct Histogram_t
{
    std::array<std::vector<int>, num_channels> hist;

    Histogram_t<frmt, T> CumulativeHistogram() const;   // running sum per channel
    void                 Reset();                       // set all bins to zero
};
```

* `num_channels` = 1 for `GRAY`, 3 for the color formats.
* Each channel vector holds `tot_elements` bins, one per representable value of `T`.

## Huffman_t&lt;frmt, T&gt;

The tables produced/consumed by Huffman encoding. Defined in [`include/Huffman.hpp`](../../../../include/Huffman.hpp).

```c++
template<ImageFormat frmt, pixel_t T>
struct Huffman_t
{
    std::unordered_map<T, std::string> table[num_channels];  // value -> bit code
    std::string                        code[num_channels];   // encoded bit stream per channel
};
```

## SobelDerivatives&lt;mag, drv&gt;

Bundle of the Sobel outputs returned by `Sobel()`. Defined in [`include/Sobel.hpp`](../../../../include/Sobel.hpp).

```c++
template<pixel_t mag = uint8_t, pixel_t drv = int16_t>
struct SobelDerivatives
{
    Image<ImageFormat::GRAY, drv>   sobel_x;    // horizontal derivative
    Image<ImageFormat::GRAY, drv>   sobel_y;    // vertical derivative
    Image<ImageFormat::GRAY, mag>   magnitude;  // gradient magnitude
    Image<ImageFormat::GRAY, float> angle;      // gradient angle
};
```

## DBSCANResult

The output of the DBSCAN clustering. Defined in [`include/DBSCAN.hpp`](../../../../include/DBSCAN.hpp).

```c++
struct DBSCANResult
{
    Image<ImageFormat::GRAY, int> labels;   // cluster id per pixel (-1 = noise)
    int  num_clusters;
    bool noise_exists;
};
```

* During processing, unvisited pixels hold the label `-2`; the final labels are
  `0 .. num_clusters - 1` for clustered pixels and `-1` for noise.



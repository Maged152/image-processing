# Functions
The `qlm` namespace is used to encapsulate various utility functions and data types for this project.

Here you can find the implementation of :
* Filters:
    * [Filter2D MxN](Functions/Filters/Filter2D)
    * [Filter2D MxN Separable](Functions/Filters/SepFilter2D)
    * [Sobel](Functions/Filters/Sobel)
    * [Gaussian](Functions/Filters/Gaussian)
    * [Box Filter](Functions/Filters/BoxFilter)
    * [Gaussian Pyramid](Functions/Filters/GaussianPyramid)
    * [Scharr](Functions/Filters/Scharr)
* Geometric Transformations:
    * [Scale](Functions/Geometric%20Transformations/Scale)
    * [Resize](Functions/Geometric%20Transformations/Resize)
    * [Rotate](Functions/Geometric%20Transformations/Rotate)
    * [Flip](Functions/Geometric%20Transformations/Flip)
    * [Transpose](Functions/Geometric%20Transformations/Transpose)
    * [Translate](Functions/Geometric%20Transformations/Translate)
    * [Warp Affine](Functions/Geometric%20Transformations/WarpAffine)
    * [Warp perspective](Functions/Geometric%20Transformations/WarpPerspective)
    * [Seam Carving](Functions/Geometric%20Transformations/SeamCarving)
* Features Detection
    * [Hough lines](Functions/Features%20Detection/HoughLines)
    * [Harris Corner](Functions/Features%20Detection/HarrisCorner)
    * [FAST](Functions/Features%20Detection/FAST)
    * [KMeans](Functions/Features%20Detection/KMeans)
    * [Match Template](Functions/Features%20Detection/MatchTemplate)
    * [DBSCAN](Functions/Features%20Detection/DBSCAN)
    * [Canny](Functions/Features%20Detection/Canny)
    * [Good Features To Track](Functions/Features%20Detection/GoodFeaturesToTrack)
    * Hough circle (backlog)
* Optical Flow
    * [Lucas-Kanade Pyramidal Optical Flow](Functions/Optical%20Flow/OpticalFlowPyrLK)
* Color Manipulation
    * [Color Conversion](Functions/Color%20Manipulation/ColorConvert)
    * [Convert Bit Depth](Functions/Color%20Manipulation/ConvertBitDepth)
    * [Flood Fill](Functions/Color%20Manipulation/FloodFill)
    * Floyd-Steinberg (backlog)
* Pixel-Wise
    * [MinMax Location](Functions/Pixel-Wise/MinMaxLoc)
    * [Histogram](Functions/Pixel-Wise/Histogram)
    * [Histogram Equalization](Functions/Pixel-Wise/EqualizeHist)
    * [Multiply](Functions/Pixel-Wise/Multiply)
    * [Subtract](Functions/Pixel-Wise/Subtract)
    * [Magnitude](Functions/Pixel-Wise/Magnitude)
    * [Min](Functions/Pixel-Wise/Min)
    * [Max](Functions/Pixel-Wise/Max)
* Get Functions
    * [Get Kernel](Functions/Get%20functions/Get%20Kernel)
    * [Get Transformation Matrix](Functions/Get%20functions/Get%20Transformation%20Matrix)
* Drawing:
    * [Draw Circle](Functions/Drawing)
    * [Draw Line](Functions/Drawing)
    * [Draw Arrowed Line](Functions/Drawing)
    * [Draw Rectangle](Functions/Drawing)
    * [Draw Cluster](Functions/Drawing)
* Compression:
    * [Huffman Encoding](Functions/Compression/Huffman)

# Data types

The core data types of the `qlm` namespace. Some of the basic types (`Image`, `Pixel`,
`BorderMode`, `Timer`, `ThreadPool`) come from separate libraries that this project
fetches and links at build time; their documentation lives in the linked repositories.

## From other libraries

* [PixelImage]([github.com/Maged152/PixelImage](https://github.com/Maged152/PixelImage)) - provides the low-level image storage:
    * [`Image<frmt, T>`](https://github.com/Maged152/PixelImage/blob/main/doc/image.md) - 2D array of pixels, pixel access, I/O, borders.
    * [`Pixel<frmt, T>`](https://github.com/Maged152/PixelImage/blob/main/doc/pixel.md) - strongly typed pixel value.
    * [`ImageFormat` / supported `pixel_t` types](https://github.com/Maged152/PixelImage/blob/main/doc/pixel_formats.md) - format definitions and the supported pixel data types.
    * `BorderMode<frmt, T>` / `BorderType` - out-of-bounds pixel policy (headers `include/image.hpp`, `include/pixel/pixel_common.hpp` of that repository).
* [ThreadPool]([github.com/Maged152/ThreadPool](https://github.com/Maged152/ThreadPool)) - header-only utilities:
    * [`qlm::Timer<duration_t>`](https://github.com/Maged152/ThreadPool/blob/main/include/timer.hpp) - execution time measurement (`Start()`, `End()`, `ElapsedString()`), from that repository's `include/timer.hpp`.
    * `qlm::ThreadPool` + `thread_safe_queue.hpp` - background parallel execution, tasks submitted with `pool.Submit(...)` (header `include/thread_pool.hpp`).

## Data types defined in this project

* [Enumerations](Data%20types/Enumerations.md)
* Classes:
    * [Basic Containers (Point, Rectangle, KeyPoint, ...)](Data%20types/Classes/Basic%20Containers/README.md)
    * [Image Statistics (Histogram_t, Huffman_t, SobelDerivatives, DBSCANResult)](Data%20types/Classes/Image%20Statistics/README.md)
    * [Function Configuration (TermCriteria)](Data%20types/Classes/Function%20Configuration/README.md)
    * [Kernels (Kernel, Kernel1D, SepKernel)](Data%20types/Classes/Kernels/README.md)
    * [Matrices (Matrix, AffineMatrix, PerspectiveMatrix)](Data%20types/Classes/Matrices/README.md)
    * [Spatial Index (KDNode, KDTree)](Data%20types/Classes/Spatial%20Index/README.md)

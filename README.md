# Image processing
Implementation of some image processing algorithms in c++
Here I'm using [stb](https://github.com/nothings/stb) to R/W images.

# Build
    $ cmake -S . -B build
    $ cmake --build build

# Functions
The `qlm` namespace is used to encapsulate various utility functions and data types for this project.

Here you can find the implementation of :
* Filters:
    * [Filter2D MxN](shakhbat_cv/doc/Functions/Filters/Filter2D)
    * [Filter2D MxN Separable](shakhbat_cv/doc/Functions/Filters/SepFilter2D)
    * [Sobel](shakhbat_cv/doc/Functions/Filters/Sobel)
    * [Gaussian](shakhbat_cv/doc/Functions/Filters/Gaussian)
    * [Box Filter](shakhbat_cv/doc/Functions/Filters/BoxFilter)
* Geometric Transformations:
    * [Scale](shakhbat_cv/doc/Functions/Geometric%20Transformations/Scale)
    * [Resize](shakhbat_cv/doc/Functions/Geometric%20Transformations/Resize)
    * [Rotate](shakhbat_cv/doc/Functions/Geometric%20Transformations/Rotate)
    * [Flip](shakhbat_cv/doc/Functions/Geometric%20Transformations/Flip)
    * [Transpose](shakhbat_cv/doc/Functions/Geometric%20Transformations/Transpose)
    * [Translate](shakhbat_cv/doc/Functions/Geometric%20Transformations/Translate)
    * [Warp Affine](shakhbat_cv/doc/Functions/Geometric%20Transformations/WarpAffine)
    * [Warp perspective](shakhbat_cv/doc/Functions/Geometric%20Transformations/WarpPerspective)
    * [Seam Carving](shakhbat_cv/doc/Functions/Geometric%20Transformations/SeamCarving)
* Features Detection
    * [Hough lines](shakhbat_cv/doc/Functions/Features%20Detection/HoughLines)
    * [Harris Corner](shakhbat_cv/doc/Functions/Features%20Detection/HarrisCorner)
    * [FAST](shakhbat_cv/doc/Functions/Features%20Detection/FAST)
    * [KMeans](shakhbat_cv/doc/Functions/Features%20Detection/KMeans)
    * [Match Template](shakhbat_cv/doc/Functions/Features%20Detection/MatchTemplate)
    * Canny (backlog)
    * Hough circle (backlog)
* Color Manipulation
    * [Color Conversion](shakhbat_cv/doc/Functions/Color%20Manipulation/ColorConvert)
    * [Convert Bit Depth](shakhbat_cv/doc/Functions/Color%20Manipulation/ConvertBitDepth)
    * [Flood Fill](shakhbat_cv/doc/Functions/Color%20Manipulation/FloodFill)
    * Floyd-Steinberg (backlog)
* Pixel-Wise
    * [MinMax Location](shakhbat_cv/doc/Functions/Pixel-Wise/MinMaxLoc)
    * Histogram (backlog)
* Get Functions
    * [Get Kernel](shakhbat_cv/doc/Functions/Get%20functions/Get%20Kernel)
    * [Get Transformation Matrix](shakhbat_cv/doc/Functions/Get%20functions/Get%20Transformation%20Matrix)
* Drawing

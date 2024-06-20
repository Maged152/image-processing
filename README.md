# Image processing
Implementation of some image processing algorithms in c++
Here I'm using [stb](https://github.com/nothings/stb) to R/W images.

# Build
    $ cmake -S . -B build -DCMAKE_GENERATOR:INTERNAL="MinGW Makefiles"
    $ cmake --build build
# Install
    $ cmake --install build --prefix <install_dir>

# Functions
The `qlm` namespace is used to encapsulate various utility functions and data types for this project.

Here you can find the implementation of :
* Filters:
    * [Filter2D MxN](doc/Functions/Filters/Filter2D)
    * [Filter2D MxN Separable](doc/Functions/Filters/SepFilter2D)
    * [Sobel](doc/Functions/Filters/Sobel)
    * [Gaussian](doc/Functions/Filters/Gaussian)
    * [Box Filter](doc/Functions/Filters/BoxFilter)
* Geometric Transformations:
    * [Scale](doc/Functions/Geometric%20Transformations/Scale)
    * [Resize](doc/Functions/Geometric%20Transformations/Resize)
    * [Rotate](doc/Functions/Geometric%20Transformations/Rotate)
    * [Flip](doc/Functions/Geometric%20Transformations/Flip)
    * [Transpose](doc/Functions/Geometric%20Transformations/Transpose)
    * [Translate](doc/Functions/Geometric%20Transformations/Translate)
    * [Warp Affine](doc/Functions/Geometric%20Transformations/WarpAffine)
    * [Warp perspective](doc/Functions/Geometric%20Transformations/WarpPerspective)
    * [Seam Carving](doc/Functions/Geometric%20Transformations/SeamCarving)
* Features Detection
    * [Hough lines](doc/Functions/Features%20Detection/HoughLines)
    * [Harris Corner](doc/Functions/Features%20Detection/HarrisCorner)
    * [FAST](doc/Functions/Features%20Detection/FAST)
    * [KMeans](doc/Functions/Features%20Detection/KMeans)
    * [Match Template](doc/Functions/Features%20Detection/MatchTemplate)
    * Canny (backlog)
    * Hough circle (backlog)
* Color Manipulation
    * [Color Conversion](doc/Functions/Color%20Manipulation/ColorConvert)
    * [Convert Bit Depth](doc/Functions/Color%20Manipulation/ConvertBitDepth)
    * [Flood Fill](doc/Functions/Color%20Manipulation/FloodFill)
    * Floyd-Steinberg (backlog)
* Pixel-Wise
    * [MinMax Location](doc/Functions/Pixel-Wise/MinMaxLoc)
    * [Histogram](doc/Functions/Pixel-Wise/Histogram)
* Get Functions
    * [Get Kernel](doc/Functions/Get%20functions/Get%20Kernel)
    * [Get Transformation Matrix](doc/Functions/Get%20functions/Get%20Transformation%20Matrix)
* Drawing

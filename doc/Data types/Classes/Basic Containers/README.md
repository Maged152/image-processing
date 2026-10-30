# Basic Containers

Simple geometric and feature container types.
Defined in [`include/common/types/containers.hpp`](../../../../include/common/types/containers.hpp)

## Point&lt;T&gt;

A 2D point with template support for both floating point and integer types.

```c++
template<class T = int>
class Point
{
public:
    T x;
    T y;

    Point();
    Point(T x, T y);

    Point<T> operator+(const Point<T>& other) const;
    Point<T> operator-(const Point<T>& other) const;
    bool     operator==(const Point<T>& other) const;
    Point<T> operator*(const float scalar) const;
    Point<T> operator/(const float scalar) const;
};
```

* `Point()` - zero initialized.
* The arithmetic operators return a new point (component-wise); multiplication and
  division accept a `float` scalar.

## Size

Size in pixels.

```c++
struct Size
{
    int width;
    int height;
};
```

## Rectangle&lt;T&gt;

An axis-aligned rectangle. `Rectangle<int>` is passed as the optional `roi` parameter
in many functions to limit the processing region.

```c++
template<class T = int>
struct Rectangle
{
    Point<T> top_left = {0, 0};
    int width = -1;
    int height = -1;
};
```

* `width <= 0` or `height <= 0` means "the whole image" - the input rectangle is
  clamped to the image bounds by the `ROI()` helper
  (`include/common/helper_functions.hpp`), which returns the borders as an
  `(x_start, y_start, x_end, y_end)` tuple.

## Line

A line segment defined by its two end points.

```c++
struct Line
{
    int x0;
    int y0;
    int x1;
    int y1;
};
```

## LinePolar

A line in polar form: distance from the origin to the closest point on the line, and
its counter-clockwise angle with the x-axis.

```c++
struct LinePolar
{
    float radius;
    float angle;
};
```

## Circle&lt;T&gt;

A circle defined by its center point and radius.

```c++
template<class T>
struct Circle
{
    Point<T> center;
    float radius;
};
```

## Cluster&lt;frmt, T&gt;

A set of points in the image associated with one label/color (the output of clustering,
e.g. KMeans).

```c++
template<ImageFormat frmt, pixel_t T>
struct Cluster
{
    std::vector<Point<int>> pixels;
    Pixel<frmt, T> color;
};
```

## KeyPoint&lt;T&gt;

A feature point state (the input/output of GoodFeaturesToTrack and OpticalFlowPyrLK).

```c++
template<class T = int>
struct KeyPoint
{
    Point<T>     point;
    float        scale;
    float        angle;
    float        response;
    float        error;
    KPStatusFlag status;
    int          level;
    int          class_id;

    KeyPoint(Point<T> pt, float response = 0, float scale = 0, float angle = -1,
             int level = 0, int class_id = -1, float error = -1,
             KPStatusFlag status = KPStatusFlag::TRACKED);
    KeyPoint();

    template<typename T2>
    operator KeyPoint<T2>() const;    // conversion between point types
};
```

* `status` marks whether the point was successfully tracked
  (`KPStatusFlag::TRACKED`) or abandoned (`KPStatusFlag::UNTRACKED`), see
  [`KPStatusFlag`](./Enumerations.md).
* The empty constructor zero-initializes `point`; the scalar members (`scale`,
  `angle`, `response`, `error`, `status`, `level`, `class_id`) are left
  uninitialized, so initialize the key-points through the parameterized constructor
  (`GoodFeaturesToTrack` does).

## Pyramid&lt;frmt, T&gt;

A Gaussian image pyramid (the output of GaussianPyramid, the internal input/output of
multi-scale functions such as OpticalFlowPyrLK).

```c++
template<ImageFormat frmt, pixel_t T>
struct Pyramid
{
    std::vector<Image<frmt, T>> layers;  // layers[0] = full resolution; each next layer is the previous one smoothed and down-scaled by scale
    int   num_layers;
    float scale;
    int   filter_size;
    float sigma;

    Pyramid(const int num_layers, const float scale, const int filter_size, const float sigma);
};
```

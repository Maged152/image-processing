# Enumerations

All enumerations of the `qlm` namespace are scoped (`enum class`).

## ImageFormat

Represents the image color formats.

|  Value       | Description              |
| ------------ | ------------------------ |
| `GRAY`       | Gray scale format.       |
| `RGB`        | Red, Green, Blue format. |
| `YCrCb`      | YCrCb format.            |
| `HSV`        | HSV format.              |
| `HLS`        | HLS format.              |

* Defined in the [PixelImage](https://github.com/Maged152/PixelImage) library
  (`include/pixel/pixel_common.hpp`), see the
  [pixel formats documentation](https://github.com/Maged152/PixelImage/blob/main/doc/pixel_formats.md).
* Every format carries an alpha channel except `YCrCb`.

## BorderType

Represents border types used in the functions
(defined in the [PixelImage](https://github.com/Maged152/PixelImage) library,
`include/image.hpp`).

| Enum Value        | Description                   |
| ----------------- | ----------------------------- |
| `BORDER_CONSTANT` | Pixels outside the image get a constant value (the default, 0). |
| `BORDER_REPLICATE`| Pixel replication.            |
| `BORDER_REFLECT`  | Border edge repeat reflection.|

## InterpolationFlag

Represents the interpolation method.

| Enum Value          | Description                     |
| ------------------- | ------------------------------- |
| `NEAREST_NEIGHBOR`  | Nearest-neighbor interpolation. |
| `BILINEAR`          | Bilinear interpolation.         |
| `BICUBIC`           | Bicubic interpolation.          |

* Defined in [`include/common/types/Interpolation.hpp`](../../include/common/types/Interpolation.hpp).

## BorderMode&lt;frmt, T&gt;

Not an enumeration, the companion of `BorderType`: it selects the border type and the
pixel used for `BORDER_CONSTANT`. Defined in the
[PixelImage](https://github.com/Maged152/PixelImage) library
(`include/image.hpp`) as

```c++
template<ImageFormat frmt, pixel_t T>
struct BorderMode
{
    BorderType border_type = BorderType::BORDER_CONSTANT;
    Pixel<frmt, T> border_pixel{};
};
```

* The default-constructed `BorderMode<frmt, T>{}` used as the default argument of the
  functions is `BORDER_CONSTANT` with a zero pixel.

---

## RotateFlag

Represents a specific rotation option.

|  Value                       | Description                           |
| -----------------------------| --------------------------------------|
| `ROTATE_90_CLOCKWISE`        | Rotate 90 degrees clockwise.          |
| `ROTATE_180`                 | Rotate 180 degrees.                   |
| `ROTATE_90_COUNTERCLOCKWISE` | Rotate 90 degrees counterclockwise.   |

## FlipFlag

Represents a specific flipping option.

| Enum Value                    | Description                               |
| ----------------------------- | ----------------------------------------- |
| `FLIP_VERTICAL_AXIS`          | Flip along the vertical axis (mirror left-right). |
| `FLIP_HORIZONTAL_AXIS`        | Flip along the horizontal axis (mirror top-bottom). |
| `FLIP_VERTICAL_HORIZONTAL_AXIS` | Flip along both the vertical and horizontal axes. |

## KMeansInit

Represents the initialization method of the KMeans cluster centers.

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `RANDOM`          | Choose initial centers uniformly among the data.     |
| `KMEANS_PLUS_PLUS`| KMeans++ initialization for better spread centers.   |

## TemplateMatchFlag

Represents the similarity metric used by template matching.

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `SQDIFF`          | Sum of squared differences.                          |
| `SQDIFF_NORMED`   | Normalized sum of squared differences.               |
| `CCORR`           | Cross correlation.                                   |
| `CCORR_NORMED`    | Normalized cross correlation.                        |

* `CCOEFF` / `CCOEFF_NORMED` (correlation coefficient) are not implemented yet.

## OrderFlag

Represents the processing order of the dimensions used by Seam Carving.

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `WIDTH_FIRST`     | Reduce/Enlarge the width first (vertical seams), then the height. |
| `HEIGHT_FIRST`    | Reduce/Enlarge the height first (horizontal seams), then the width. |

## EnergyFlag

Represents the energy function used by Seam Carving.

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `FORWARD`         | Forward energy function.                             |
| `BACKWARD`        | Backward energy function (gradient magnitude).       |

## OverFlowFlag

Represents how the arithmetical functions handle values that exceed the pixel type
range (e.g. `Multiply`).

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `SATURATE`        | Clamp the result into the type range (default).      |
| `WRAP`            | Keep the raw (possibly overflowing) value.           |

## KPStatusFlag

Represents the tracking state of a key-point (see [`KeyPoint`](./Classes/Basic%20Containers/README.md#keypointt)).

| Enum Value        | Description                                          |
| ----------------- | ---------------------------------------------------- |
| `TRACKED`         | The key-point has a valid tracked position.          |
| `UNTRACKED`       | The key-point was abandoned by the tracker.          |

---

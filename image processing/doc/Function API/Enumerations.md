# Enumerations


## ImageFormat

Represents various color formats for images.

|  Value       | Description              |
| ------------ | ------------------------ |
| `GRAY`       | Gray scale format.       |
| `RGB`        | Red, Green, Blue format. |
| `YCrCb`      | YCrCb format.            |
| `HSV`        | HSV format.              |
| `HLS`        | HLS format.              |

## BorderType

Represents border types used in the functions.

| Enum Value        | Description                   |
| ----------------- | ----------------------------- |
| `BORDER_CONSTANT` | Constant border replication.  |
| `BORDER_REPLICATE`| Border pixel replication.     |
| `BORDER_REFLECT`  | Border reflection.            |

## InterpolationFlag

Represents interpolation method.

| Enum Value          | Description                     |
| ------------------- | ------------------------------- |
| `NEAREST_NEIGHBOR`  | Nearest-neighbor interpolation. |
| `BILINEAR`          | Bilinear interpolation.         |
| `BICUBIC`           | Bicubic interpolation.          |



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
| `FLIP_VERTICAL_AXIS`          | Flip along the vertical axis.              |
| `FLIP_HORIZONTAL_AXIS`        | Flip along the horizontal axis.            |
| `FLIP_VERTICAL_HORIZONTAL_AXIS` | Flip along both the vertical and horizontal axes. |


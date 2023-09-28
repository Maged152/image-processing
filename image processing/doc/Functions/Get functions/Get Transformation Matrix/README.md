# Get Transformation Matrix

## Get Rotation Matrix

### Description
The `GetRotationMatrix` function is used to obtain a transformation matrix that represents
a 2D rotation around a specified center point by a given angle and scaling factor.


### C++ API
```c++
    TransformationMatrix GetRotationMatrix(const Point<int>& center,
										   const float angle,
										   const float scale);
```
### Parameters

| Name     | Type         | Description                                |
| -------- | -------------|------------------------------------------- |
| `center` | 'Point<int>' |The center point for the rotation           |
| `angle`  | 'float'      |The angle of rotation in degrees.           |
| `scale`  | 'float'      | The scaling factor for the transformation. |

### Return Value
The function returns a transformation matrix representing transformation of type `TransformationMatrix`.

* [Implementation](../../../../code/WarpAffine/WarpAffine.cpp)

## Get Affine Transform

### Description
The `GetAffineTransform` function is used to  obtain a transformation matrix that represents
an affine transformation mapping between three source points and their corresponding
destination points.


### C++ API
```c++
    	TransformationMatrix GetAffineTransform(const Point<int> src[3],
											    const Point<int> dst[3]);
```
### Parameters

| Name  |      Type      | Description                            |
| ----- | ---------------|--------------------------------------- |
| `src` | 'Point<int>[3]'|An array of three source points         |
| `dst` | 'Point<int>[3]'| An array of three destination points.  |


### Return Value
The function returns a transformation matrix representing transformation of type `TransformationMatrix`.

* [Implementation](../../../../code/WarpAffine/WarpAffine.cpp)

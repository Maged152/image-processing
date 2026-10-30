# Matrices

The float matrix types used by the linear solvers and the geometric transformations.

## Matrix

A small dynamically allocated float matrix used by the linear solvers and the
transform getters. Defined in [`include/common/types/matrix.hpp`](../../../../include/common/types/matrix.hpp).

```c++
class Matrix
{
public:
    Matrix();                                   // empty
    Matrix(int rows, int columns);              // allocates
    Matrix(float* data, int rows, int columns); // adopts an existing buffer (no ownership)

    float Get(int row, int col) const;          // out of range -> signaling NaN
    float Get(int index) const;                 // row-major index
    void  Set(int row, int col, float value);   // out of range -> ignored
    void  Set(int index, float value);

    int Rows() const;
    int Columns() const;

    void Print(int digits = 5) const;
    void RandomInit(float min_value, float max_value);
    void SetToNull();   // detach the buffer without deleting it
};
```

* **Note:** `SetToNull()` exists so a stack array wrapped by the pointer constructor
  is not deleted by the destructor (`float aug[3][4]`), i.e. the matrix does not own
  buffers it did not allocate.

## TransformationMatrix&lt;num_rows, num_cols&gt;

A fixed-size (compile time) float matrix, base of the concrete transformation types.
Defined in
[`include/common/types/TransformationMatrix.hpp`](../../../../include/common/types/TransformationMatrix.hpp).

```c++
template<int num_rows, int num_cols>
class TransformationMatrix
{
public:
    TransformationMatrix();                             // identity matrix

    float GetElement(int r, int c) const;               // out of range -> 0.0f
    float GetElement(int i) const;                      // row-major index
    void  SetElement(int r, int c, float value);
    void  SetElement(int i, float value);

    int  NumberRows() const;
    int  NumberColumns() const;
    void Print(int digits = 5) const;

    bool operator==(const TransformationMatrix& other) const;
    TransformationMatrix& operator=(const TransformationMatrix& other);
};
```

## AffineMatrix

A 2x3 affine transformation matrix
(`AffineMatrix : TransformationMatrix<2, 3>`), usable directly with `WarpAffine`.

```c++
class AffineMatrix : public TransformationMatrix<2, 3>
{
public:
    AffineMatrix();   // identity
    AffineMatrix(float m00, float m01, float m02,
                 float m10, float m11, float m12);
};
```

## PerspectiveMatrix

A 3x3 projective transformation matrix
(`PerspectiveMatrix : TransformationMatrix<3, 3>`), usable directly with
`WarpPerspective` (the element (2, 2) is usually kept 1).

```c++
class PerspectiveMatrix : public TransformationMatrix<3, 3>
{
public:
    PerspectiveMatrix();   // identity
    PerspectiveMatrix(float m00, float m01, float m02,
                      float m10, float m11, float m12,
                      float m20, float m21, float m22);
};
```

* The matrix types are obtained with `GetRotationMatrix`, `GetAffineTransform`,
  `GetPerspectiveTransform`
  (see [Get Transformation Matrix](../../../Functions/Get%20functions/Get%20Transformation%20Matrix)).



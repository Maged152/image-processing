# Kernels

The kernel types used by the filtering functions.
Defined in [`include/common/types/Kernel.hpp`](../../../../include/common/types/Kernel.hpp)

## Kernel

A 2D float kernel used by `Filter2D`.

```c++
class Kernel
{
public:
    Kernel(int width, int height);          // allocates height * width coefficients
    Kernel(const Kernel& other);            // deep copy
    Kernel(Kernel&& other) noexcept;        // move

    float Get(int x, int y) const;          // out of range -> 0.0f
    void  Set(int x, int y, float value);   // out of range -> ignored

    int Width() const;
    int Height() const;
};
```

## Kernel1D

A one-dimensional kernel (a `Kernel` of width 1).

```c++
class Kernel1D : private Kernel
{
public:
    Kernel1D(int length);

    float Get(int idx) const;
    void  Set(int idx, float value);

    int Length() const;
};
```

## SepKernel

The two 1D kernels of a separable 2D filter. Defined in
[`include/common/types/Kernel.hpp`](../../../../include/common/types/Kernel.hpp).

```c++
class SepKernel
{
public:
    Kernel1D x_ker;   // applied along x (after the y pass)
    Kernel1D y_ker;   // applied along y (first pass)

    SepKernel(int len_x, int len_y);
};
```

* `SepFilter2D` applies the y-kernel first and the x-kernel second:
  `out = x_ker(y_ker(in))`.
* Ready-made kernels are obtained with `GetGaussianKernel`, `GetDerivXKernel`,
  `GetDerivYKernel` and `GetScharrXKernel`, `GetScharrYKernel`
  (see [Get Kernel](../../../Functions/Get%20functions/Get%20Kernel)).



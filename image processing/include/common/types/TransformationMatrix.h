#pragma once

namespace qlm
{
    template<int num_rows, int num_cols>
    class TransformationMatrix
    {
    protected:
        float data[num_rows * num_cols];
        static const int rows = num_rows;
        static const int cols = num_cols;

    public:
        // Constructors
        TransformationMatrix();
        // Overloaded operators
        bool operator==(const TransformationMatrix& other) const;

        TransformationMatrix& operator=(const TransformationMatrix& rhs);
    public:
        // Getter and setter methods for x and y inputs
        float GetElement(int r, int c) const;
        void SetElement(int r, int c, float value);

        // Getter and setter methods for individual elements i
        float GetElement(int i) const;
        void SetElement(int i, float value);

        int NumberRows() const
        {
            return rows;
        }
        int NumberColumns() const
        {
            return cols;
        }
    };

    /////////////////////////////////////////////////////////////////
    class AffineMatrix : public TransformationMatrix<2, 3>
    {
    public:
        AffineMatrix() : TransformationMatrix()
        {}
        AffineMatrix(float m00, float m01, float m02,
                     float m10, float m11, float m12)
        {
            data[0] = m00;
            data[1] = m01;
            data[2] = m02;
            data[3] = m10;
            data[4] = m11;
            data[5] = m12;
        }
    };

    /////////////////////////////////////////////
    class PerspectiveMatrix : public TransformationMatrix<3, 3>
    {
    public:
        PerspectiveMatrix() : TransformationMatrix()
        {}
        PerspectiveMatrix(float m00, float m01, float m02,
                          float m10, float m11, float m12,
                          float m20, float m21, float m22)
        {
            data[0] = m00;
            data[1] = m01;
            data[2] = m02;
            data[3] = m10;
            data[4] = m11;
            data[5] = m12;
            data[6] = m20;
            data[7] = m21;
            data[8] = m22;
        }
     
    };
}
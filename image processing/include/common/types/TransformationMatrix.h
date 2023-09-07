#pragma once

namespace qlm
{
    class TransformationMatrix
    {
    private:
        float data[6];
        static const int rows = 2;
        static const int cols = 3;

    public:
        // Constructors
        TransformationMatrix();
        TransformationMatrix(float m00, float m01, float m02,
            float m10, float m11, float m12);

        // Getter and setter methods for x and y inputs
        float GetElement(int r, int c) const;
        void SetElement(int r, int c, float value);

        // Getter and setter methods for individual elements i
        float GetElement(int i) const;
        void SetElement(int i, float value);

        int NumberRows() const;
        int NumberColumns() const;
        // Overloaded operators
        bool operator==(const TransformationMatrix& other) const;

        TransformationMatrix& operator=(const TransformationMatrix& rhs);
    };
}
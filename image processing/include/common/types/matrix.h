#pragma once

#include <limits>


namespace qlm
{
    class Matrix 
    {
    private:
        float* data;
        int columns;
        int rows;


    public:
        // Default constructor
        Matrix() : data(nullptr), columns(0), rows(0)
        {}
        // Parameterized constructor
        Matrix(int r, int c) : columns(c), rows(r)
        {
            data = new float[columns * rows];
        }
        
        Matrix(float * data, int r, int c) : data(data), columns(c), rows(r)
        {}
        // Copy constructor
        Matrix(const Matrix& other) : columns(other.columns), rows(other.rows)
        {
            data = new float[columns * rows];
            for (int i = 0; i < columns * rows; ++i) {
                data[i] = other.data[i];
            }
        }
        // Destructor
        ~Matrix()
        {
            rows = columns = 0;
            if (data != nullptr)
                delete[] data;
        }
        // move and = operator
    public:
        // Setter for individual element
        void Set(int row, int col, float value)
        {
            if (row >= 0 && row < rows && col >= 0 && col < columns)
            {
                data[row * columns + col] = value;
            }
        }
        void Set(int i, float value) {
            if (i >= 0 && i < columns * rows)
            {
                data[i] = value;
            }
        }
        // Getter for individual element
        float Get(int row, int col) const
        {
            if (row >= 0 && row < rows && col >= 0 && col < columns)
            {
                return data[row * columns + col];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        float Get(int i) const
        {
            if (i >= 0 && i < columns * rows)
            {
                return data[i];
            }
            return std::numeric_limits<float>::signaling_NaN();
        }
        // Getter for columns
        int Columns() const
        {
            return columns;
        }
        // Getter for rows
        int Rows() const
        {
            return rows;
        }
    public:
        // print matrix
        void Print(int digits = 5) const;
        // random initialization
        void RandomInit(const float min_value, const float max_value);
    };
}
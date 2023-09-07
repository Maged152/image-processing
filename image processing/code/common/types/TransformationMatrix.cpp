#include "common/types/TransformationMatrix.h"

namespace qlm
{

    // Default constructor
    TransformationMatrix::TransformationMatrix() {
        // Initialize the matrix to an identity matrix
        data[0] = 1.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = 1.0f;
        data[5] = 0.0f;
    }

    // Parameterized constructor
    TransformationMatrix::TransformationMatrix(float m00, float m01, float m02,
        float m10, float m11, float m12) {
        data[0] = m00;
        data[1] = m01;
        data[2] = m02;
        data[3] = m10;
        data[4] = m11;
        data[5] = m12;
    }

    // Getter
    float TransformationMatrix::GetElement(int r, int c) const 
    {
        if (r >= 0 && r < rows && c >= 0 && c < cols) 
        {
            return data[r * cols + c];
        }
        else 
        {
            return 0.0f; // Return 0 as a default value in case of an error.
        }
    }
    
    float TransformationMatrix::GetElement(int i) const 
    {
        if (i >= 0 && i < rows * cols) 
        {
            return data[i];
        }
        else 
        {
            return 0.0f; // Return 0 as a default value in case of an error.
        }
    }

    // Setter
    void TransformationMatrix::SetElement(int r, int c, float value) 
    {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
        {
            data[r * cols + c] = value;
        }
    }

    void TransformationMatrix::SetElement(int i, float value) 
    {
        if (i >= 0 && i < rows * cols)
        {
            data[i] = value;
        }
    }

   // matrix dimensions
    int  TransformationMatrix::NumberRows() const
    {
        return rows;
    }

    int  TransformationMatrix::NumberColumns() const
    {
        return cols;
    }

    // operators
    // // Overloaded equality operator
    bool TransformationMatrix::operator==(const TransformationMatrix& other) const
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if (this->GetElement(r, c) != other.GetElement(r, c))
                {
                    return false;
                }
            }
        }
        return true;
    }

    // Assignment operator
    TransformationMatrix& TransformationMatrix::operator=(const TransformationMatrix& rhs) {
        if (this == &rhs) {
            // Self-assignment, do nothing
            return *this;
        }

        // Copy the elements from rhs to lhs
        for (int i = 0; i < rows * cols; i++) {
            data[i] = rhs.data[i];
        }

        // Return the modified object
        return *this;
    }

}
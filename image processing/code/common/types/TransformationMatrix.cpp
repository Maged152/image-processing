#include "common/types/TransformationMatrix.h"
#include <iostream>
#include <iomanip>
#include <cmath>

namespace qlm
{

    // Default constructor
    template<int num_rows, int num_cols>
    TransformationMatrix<num_rows, num_cols>::TransformationMatrix() {
        // Initialize the matrix to an identity matrix
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                if (r == c)
                {
                    this->SetElement(r, c, 1.0f);
                }
                else
                {
                    this->SetElement(r, c, 0.0f);
                }
            }
        }
    }

    // print the matrix
    template<int num_rows, int num_cols>
    void TransformationMatrix<num_rows, num_cols>::Print(int digits) const
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < cols; c++)
            {
                int print_digits = digits;
                float element = this->GetElement(r, c);

                if (element != 0)
                {
                    int fixed_digits = static_cast<int>(std::log10(std::abs(element))) + 1;
                    print_digits = fixed_digits >= digits ? 0 : fixed_digits - digits;
                }

                std::cout << std::fixed << std::setprecision(print_digits) << element << " ";
            }
            std::cout << std::endl;
        }
    }

    // Getter
    template<int num_rows, int num_cols>
    float TransformationMatrix<num_rows, num_cols>::GetElement(int r, int c) const
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
    
    template<int num_rows, int num_cols>
    float TransformationMatrix<num_rows, num_cols>::GetElement(int i) const
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
    template<int num_rows, int num_cols>
    void TransformationMatrix<num_rows, num_cols>::SetElement(int r, int c, float value)
    {
        if (r >= 0 && r < rows && c >= 0 && c < cols)
        {
            data[r * cols + c] = value;
        }
    }

    template<int num_rows, int num_cols>
    void TransformationMatrix<num_rows, num_cols>::SetElement(int i, float value)
    {
        if (i >= 0 && i < rows * cols)
        {
            data[i] = value;
        }
    }

    // operators
    // Overloaded equality operator
    template<int num_rows, int num_cols>
    bool TransformationMatrix<num_rows, num_cols>::operator==(const TransformationMatrix<num_rows, num_cols>& other) const
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
    template<int num_rows, int num_cols>
    TransformationMatrix<num_rows, num_cols>& TransformationMatrix<num_rows, num_cols>::operator=(const TransformationMatrix<num_rows, num_cols>& rhs) {
        if (this == &rhs) {
            // Self-assignment, do nothing
            return *this;
        }

        // Copy the elements from rhs to this
        for (int i = 0; i < rows * cols; i++) {
            data[i] = rhs.data[i];
        }

        // Return the modified object
        return *this;
    }


    template class TransformationMatrix<2, 3>;
    template class TransformationMatrix<3, 3>;
}
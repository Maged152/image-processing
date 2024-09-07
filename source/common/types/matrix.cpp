#include "common/types/matrix.hpp"
#include <iostream>
#include <random>
#include <iomanip>
#include <cmath>

namespace qlm
{
    void Matrix::Print(int digits) const
    {
        for (int r = 0; r < rows; r++)
        {
            for (int c = 0; c < columns; c++)
            {
                int print_digits = digits;
                float element = this->Get(r, c);

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

    void Matrix::RandomInit(const float min_value, const float max_value)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min_value, max_value);

        for (int i = 0; i < columns * rows; i++)
        {
            this->Set(i, dis(gen));
        }
    }
}
#pragma once

#include "shakhbat_cv.h"
#include <string>
#include <windows.h>
#include <array>
#include <filesystem>

#define CONSOLE_COLOR_BLACK 0
#define CONSOLE_COLOR_BLUE 1
#define CONSOLE_COLOR_GREEN 2
#define CONSOLE_COLOR_CYAN 3
#define CONSOLE_COLOR_RED 4
#define CONSOLE_COLOR_MAGENTA 5
#define CONSOLE_COLOR_YELLOW 6
#define CONSOLE_COLOR_WHITE 7
#define CONSOLE_COLOR_GRAY 8
#define CONSOLE_COLOR_LIGHT_BLUE 9
#define CONSOLE_COLOR_LIGHT_GREEN 10
#define CONSOLE_COLOR_LIGHT_CYAN 11
#define CONSOLE_COLOR_LIGHT_RED 12
#define CONSOLE_COLOR_LIGHT_MAGENTA 13
#define CONSOLE_COLOR_LIGHT_YELLOW 14
#define CONSOLE_COLOR_BRIGHT_WHITE 15

namespace test
{
    const std::string example_folder = "../../doc/Functions/";

    inline void PrintTestResults(const std::string& name,
                                 const bool res, 
                                 const qlm::Timer<qlm::usec>& time,
                                 const float normalization_factor,
                                 const HANDLE& col_handle)
    {
        // test name
        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_BLUE);
        std::cout << name << " : ";
        // time taken
        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_YELLOW);
        std::cout << time.duration.count() / normalization_factor << " usec : ";

        if (res)
        {
            SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
            std::cout << "PASSED\n";
        }
        else
        {
            SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
            std::cout << "FAILED\n";
        }

        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
    }


    template <size_t size>
    inline void PrintTestResults(const std::string& name,
        const std::array<bool, size>& res,
        const std::array<qlm::Timer<qlm::usec>, size>& time,
        const std::array<float, size>& normalization_factor,
        const HANDLE& col_handle)
    {
        // test name
        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_BLUE);
        std::cout << name << " : ";
        // time taken
        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_YELLOW);
        std::cout << "[";
        for (int i = 0; i < time.size(); i++)
        {
            std::cout << time[i].duration.count() / normalization_factor[i] << ", ";
        }
        std::cout << "\b\b]usec : ";

        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
        std::cout << "[";

        for (auto& r : res)
        {
            if (r)
            {
                SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
                std::cout << "PASSED, ";
            }
            else
            {
                SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_RED);
                std::cout << "FAILED, ";
            }
        }

        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_GREEN);
        std::cout << "\b\b]\n";

        SetConsoleTextAttribute(col_handle, CONSOLE_COLOR_WHITE);
    }

    // run all tests
    bool Test_All();

    // compare two pixels
    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    bool Test_ComparePixels(const qlm::Pixel<frmt, T>& in1,
                            const qlm::Pixel<frmt, T>& in2,
                            const qlm::Pixel<frmt, T>& threshold = qlm::Pixel<frmt, T> {})
    {
        auto abs_diff = qlm::AbsDiff(in1, in2);

        if constexpr (frmt == qlm::ImageFormat::GRAY)
        {
            if (abs_diff.v <= threshold.v)
            {
                return true;
            }
        }
        else if constexpr (frmt == qlm::ImageFormat::RGB)
        {
            
            if (abs_diff.r <= threshold.r &&
                abs_diff.g <= threshold.g &&
                abs_diff.b <= threshold.b)
            {
                
                return true;
            }
            /*else
            {
                std::cout << (int)abs_diff.r << "  " << (int)abs_diff.g << " "
                    << (int)abs_diff.b << "  " << "\n";
            }*/
        }
        else if constexpr (frmt == qlm::ImageFormat::HLS)
        {
            if (abs_diff.h <= threshold.h &&
                abs_diff.l <= threshold.l &&
                abs_diff.d <= threshold.s)
            {
                return true;
            }
        }
        else
        {
            if (abs_diff.h <= threshold.h &&
                abs_diff.v <= threshold.v &&
                abs_diff.s <= threshold.s)
            {
                return true;
            }
        }

        return false;
    }

    // compare two images
    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    bool Test_CompareImages(const qlm::Image<frmt, T>& in1,
                            const qlm::Image<frmt, T>& in2,
                            const qlm::Pixel<frmt, T>& threshold = qlm::Pixel<frmt, T>{})
    {
        // check the dimensions
        if (in1.width != in2.width || in1.height != in2.height)
        {
            std::cout << "the dimensions are not the same" 
                      << in1.width << "x" << in1.height << " vs "
                      << in2.width << "x" << in2.height << "\n" ;
            return false;
        }
        // check the pixels
        for (int y = 0; y < in1.height; y++)
        {
            for (int x = 0; x < in1.width; x++)
            {
                bool res = Test_ComparePixels(in1.GetPixel(x, y), in2.GetPixel(x, y), threshold);

                if (res == false)
                {
                    return false;
                }
            }
        }

        return true;
    }
}
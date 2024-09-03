#pragma once

#include "shakhbat_cv.h"
#include "gtest/gtest.h"
#include <string>
#include <windows.h>
#include <array>
#include <filesystem>

#define ANSI_TXT_GRN "\033[0;32m"
#define ANSI_TXT_RED "\033[0;31m"
#define ANSI_TXT_MGT "\033[0;35m" //Magenta
#define ANSI_TXT_DFT "\033[0;0m" //Console default

#define GTEST_BOX      "[Parameters] "
#define GTEST_BOX_TIME "[Time      ] "
#define GTEST_BOX_FAST "[Faster    ] "
#define GTEST_BOX_SLOW "[Slower    ] "

#define COUT_GTEST ANSI_TXT_GRN << GTEST_BOX 
#define COUT_GTEST_TIME ANSI_TXT_GRN << GTEST_BOX_TIME
#define COUT_GTEST_FAST ANSI_TXT_GRN << GTEST_BOX_FAST 
#define COUT_GTEST_SLOW ANSI_TXT_GRN << GTEST_BOX_SLOW 

#define COUT_GTEST_MGT COUT_GTEST << ANSI_TXT_MGT
#define COUT_GTEST_MGT_TIME COUT_GTEST_TIME << ANSI_TXT_MGT
#define COUT_GTEST_GRN_FAST COUT_GTEST_FAST << ANSI_TXT_GRN
#define COUT_GTEST_RED_SLOW COUT_GTEST_SLOW << ANSI_TXT_RED
namespace test
{
    const std::string example_folder = "doc/Functions/";

    void PrintTime(const qlm::Timer<qlm::usec>& time)
	{
		std::cout << COUT_GTEST_MGT_TIME << "Time"
			                             << " = "
			                             << time.Duration()
			                             << " usec"
		                                 << ANSI_TXT_DFT << std::endl;
    }

    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    qlm::Image<frmt, T> ReReadImage(qlm::Image<frmt, T>& out)
    {
    const bool save_out = out.SaveToFile("out.jpg");
    EXPECT_EQ(save_out, true);

    // read output image
    qlm::Image<qlm::ImageFormat::RGB, uint8_t> cur;
    const bool reread_out = cur.LoadFromFile("out.jpg");
    EXPECT_EQ(reread_out, true);

    // delete output image
    const bool remove_out = std::filesystem::remove("out.jpg");
    EXPECT_EQ(remove_out, true);

    return cur;
    }

     // compare two pixels
    template<qlm::ImageFormat frmt, qlm::pixel_t T>
    bool ComparePixels(const qlm::Pixel<frmt, T>& in1,
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
    void CompareImages(const qlm::Image<frmt, T>& in1,
                       const qlm::Image<frmt, T>& in2,
                       const qlm::Pixel<frmt, T>& threshold = qlm::Pixel<frmt, T>{})
    {
        // check the dimensions
        EXPECT_EQ(in1.width, in2.width);
        EXPECT_EQ(in1.height, in2.height);
       
        // check the pixels
        for (int y = 0; y < in1.height; y++)
        {
            for (int x = 0; x < in1.width; x++)
            {
                const bool res = ComparePixels(in1.GetPixel(x, y), in2.GetPixel(x, y), threshold);
                EXPECT_EQ(res, true);
            }
        }
    }
}
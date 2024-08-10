#include "common/WebCam.h"
#include <iostream>

namespace qlm
{
    WebCam::WebCam(int width, int height, int device) : device_num(device), num_cameras(-1)
    {
        capture.mWidth = width;
        capture.mHeight = height;
        capture.mTargetBuf = new int [width * height];
    }

    bool WebCam::Init()
    {
        num_cameras = setupESCAPI();
        if (num_cameras == 0)
        {
            std::cerr << "initialization failure or no devices found." << std::endl;
            return false;
        }

        if (device_num >= num_cameras) 
        {
            std::cerr << "Invalid device index." << std::endl;
            return false;
        }

        if (initCapture(device_num, &capture) == 0) 
        {
            std::cerr << "Capture initialization failed - device may be in use." << std::endl;
            return false;
        }

        return true;
    }

     Image<ImageFormat::RGB, uint8_t> qlm::WebCam::Capture()
    {
        doCapture(device_num);
        while (isCaptureDone(device_num) == 0) 
        { 
            // Wait for capture to complete
        }

        Image<ImageFormat::RGB, uint8_t> frame {(size_t)capture.mWidth, (size_t)capture.mHeight};

        // Get pixel
        union RGBint
        {
            int rgb;
            unsigned char c[4];
        };
        RGBint col;

        for (int i = 0; i < capture.mWidth * capture.mHeight; i++)
        {
            col.rgb =  capture.mTargetBuf[i];
            frame.SetPixel(i, {col.c[2], col.c[1], col.c[0]});
        }

        return frame;
    }
}
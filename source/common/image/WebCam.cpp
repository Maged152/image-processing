#include "WebCam.h"
#include <iostream>

namespace qlm
{
    WebCam::WebCam(int width, int height, int device) : cam_width(width), cam_height(height), device_num(-1), num_cameras(-1)
    {}

    WebCam::Init()
    {
        num_cameras = setupESCAPI();
        if (num_cameras == 0)
        {
            std::cerr << "initialization failure or no devices found." << std::endl;
            return false;
        }

        if (device >= num_cameras) 
        {
            std::cerr << "Invalid device index." << std::endl;
            return false;
        }

        device_num = device;

        if (initCapture(device, &capture) == 0) 
        {
            std::cerr << "Capture initialization failed - device may be in use." << std::endl;
            return false;
        }

        return true;
    }

}
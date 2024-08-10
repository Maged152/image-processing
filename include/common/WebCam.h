#pragma once

#include "common/image.h"
#include "escapi/escapi.h"
namespace qlm
{
    class WebCam
    {
        private:
        int num_cameras;
        int device_num;
        SimpleCapParams capture;

        public:
        WebCam(int width, int height, int device);
        bool Init();
        Image<ImageFormat::RGB, uint8_t> Capture();
    };

}
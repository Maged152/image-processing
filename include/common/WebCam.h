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
        int cam_width;
        int cam_height;

        public:
        WebCam(int width, int height, int device);
        bool Init();
    }

}
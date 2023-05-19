#pragma once

#include "common/types.h"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    void DrawLine(Image<frmt, T>& in, const Line& line, const Pixel<frmt, T>& c)
    {
        int width = in.Width();
        int height = in.Height();

        auto [x1, y1, x2, y2] = line;
        int dx = x2 - x1;
        int dy = y2 - y1;

        // If both of the differences are 0 there will be a division by 0 below.
        if (dx == 0 && dy == 0)
        {
            if (0 <= x1 && x1 < width && 0 <= y1 && y1 < height)
            {
                // just a point
                in.SetPixel(x1, y1, c);
            }
            return;
        }

        if (std::abs(dx) > std::abs(dy))
        {
            if (x1 > x2)
            {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            for (int x = x1; x <= x2; ++x)
            {
                int y = dy * (x - x1) / dx + y1;
                if (0 <= y && y < height && 0 <= x && x < width)
                {
                    in.SetPixel(x, y, c);
                }
            }
        }
        else {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            for (int y = y1; y <= y2; ++y)
            {
                int x = dx * (y - y1) / dy + x1;
                if (0 <= y && y < height && 0 <= x && x < width)
                {
                    in.SetPixel(x, y, c);
                }
            }
        }
    }

    template<ImageFormat frmt, pixel_t T>
    void DrawLine(Image<frmt, T>& in, const LinePolar& line, const  Pixel<frmt, T>& color)
    {
        int width = in.Width();
        int height = in.Height();

        auto theta = line.angle;
        auto r = line.radius;

        auto cos_angle = std::cos(theta); 
        auto sin_angle = std::sin(theta);

        int x0 = cos_angle * r; 
        int y0 = sin_angle * r;

        int x1 = int(x0 + 1000 * (-sin_angle));
        int y1 = int(y0 + 1000 * (cos_angle));

        int x2 = int(x0 - 1000 * (-sin_angle));
        int y2 = int(y0 - 1000 * (cos_angle));

        Line l{x1 ,y1 ,x2 ,y2};
        DrawLine(in, l, color);
    }

}
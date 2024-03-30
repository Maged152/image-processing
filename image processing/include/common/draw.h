#pragma once

#include "common/types.h"

namespace qlm
{
    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> DrawCircle(const Image<frmt, T>& in, const Circle<int>& circle, const Pixel<frmt, T>& color, int thickness = 1)
    {
        Image<frmt, T> out = in;

        const int center_x = circle.center.x;
        const int center_y = circle.center.y;
        const int radius = circle.radius;

        if (thickness <= 0) {
            return out;
        }

        // Draw the circle boundary at all thickness levels
        for (int t = 0; t < thickness; t++) 
        {
            int sign = t % 2 ? 1 : -1;
            int side = sign * (std::ceil(t / 2.0f));
            int x = 0;
            int y = radius + side;
            int d = 3 - 2 * (radius + side);

            while (y >= x) 
            {
                // Draw pixels at all thickness levels
                out.SetPixel(center_x + x, center_y + y, color);
                out.SetPixel(center_x - x, center_y + y, color);
                out.SetPixel(center_x + x, center_y - y, color);
                out.SetPixel(center_x - x, center_y - y, color);
                out.SetPixel(center_x + y, center_y + x, color);
                out.SetPixel(center_x - y, center_y + x, color);
                out.SetPixel(center_x + y, center_y - x, color);
                out.SetPixel(center_x - y, center_y - x, color);

                if (d < 0) 
                {
                    d = d + 4 * x + 6;
                }
                else 
                {
                    d = d + 4 * (x - y) + 10;
                    y--;
                }
                x++;
            }
        }
        return out;
    }

    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> DrawLine(const Image<frmt, T>& in, const Line& line, const Pixel<frmt, T>& c)
    {
        Image<frmt, T> out = in;

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
                out.SetPixel(x1, y1, c);
            }
            return out;
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
                    out.SetPixel(x, y, c);
                }
            }
        }
        else 
        {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            for (int y = y1; y <= y2; ++y)
            {
                int x = dx * (y - y1) / dy + x1;
                if (0 <= y && y < height && 0 <= x && x < width)
                {
                    out.SetPixel(x, y, c);
                }
            }
        }
        return out;
    }

    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> DrawLine(const Image<frmt, T>& in, const LinePolar& line, const  Pixel<frmt, T>& color)
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
        return DrawLine(in, l, color);
    }

    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> DrawRectangle(const Image<frmt, T>& in, const Rectangle& rec, const Pixel<frmt, T>& c) 
    {
        // Copy the input image to avoid modifying it
        Image<frmt, T> out = in;

        // Use DrawLine to draw all four sides of the rectangle
        out = DrawLine(out, Line(rec.top_left.x, rec.top_left.y, rec.top_left.x + rec.width, rec.top_left.y), c);  // Top line
        out = DrawLine(out, Line(rec.top_left.x, rec.top_left.y + rec.height, rec.top_left.x + rec.width, rec.top_left.y + rec.height), c);  // Bottom line
        out = DrawLine(out, Line(rec.top_left.x + rec.width, rec.top_left.y, rec.top_left.x + rec.width, rec.top_left.y + rec.height), c);  // Right line
        out = DrawLine(out, Line(rec.top_left.x, rec.top_left.y, rec.top_left.x, rec.top_left.y + rec.height), c);  // Left line

        return out;
    }


    template<ImageFormat frmt, pixel_t T>
    Image<frmt, T> DrawCluster(const Image<frmt, T>& in, const std::vector<Cluster<frmt, T>>& clusters)
    {
        Image<frmt, T> out;
        out.create(in.Width(), in.Height());

        for (int c = 0; c < clusters.size(); c++)
        {
            auto& color = clusters[c].color;

            for (auto& idx : clusters[c].pixels)
            {
                out.SetPixel(idx.x, idx.y, color);
            }
        }

        return out;
    }

}
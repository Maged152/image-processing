#pragma once
#include "SFML/Graphics.hpp"
#include "types.h"

namespace qlm
{

    void DrawLine(sf::Image& in, const Line& line, const sf::Color& c)
    {
        int width = in.getSize().x;
        int height = in.getSize().y;

        auto [x1, y1, x2, y2] = line;
        int dx = x2 - x1;
        int dy = y2 - y1;

        // If both of the differences are 0 there will be a division by 0 below.
        if (dx == 0 && dy == 0)
        {
            if (0 <= x1 && x1 < width && 0 <= y1 && y1 < height)
            {
                // just a point
                in.setPixel(x1, y1, c);
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

            // Cull out invisible line
            if (x1 > width || x2 < 0) return;

            // Clamp the line to the boundaries
            if (x1 < 0) x1 = 0;
            if (x2 >= width) x2 = width - 1;

            for (int x = x1; x <= x2; ++x)
            {
                int y = dy * (x - x1) / dx + y1;
                if (0 <= y && y < height)
                {
                    in.setPixel(x, y, c);
                }
            }
        }
        else {
            if (y1 > y2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }

            // Cull out invisible line
            if (y1 > height || y2 < 0) return;

            // Clamp the line to the boundaries
            if (y1 < 0) y1 = 0;
            if (y2 >= height) y2 = height - 1;

            for (int y = y1; y <= y2; ++y)
            {
                int x = dx * (y - y1) / dy + x1;
                if (0 <= x && x < width)
                {
                    in.setPixel(x, y, c);
                }
            }
        }
    }

    void DrawLine(sf::Image& in, const LinePolar& line, const sf::Color& color)
    {
        int width = in.getSize().x;
        int height = in.getSize().y;
        auto theta = line.angle;
        auto r = line.radius;
        auto c = std::cos(theta), s = std::sin(theta);
        int x0 = c * r, y0 = s * r;
        int x1 = std::clamp(int(x0 + 1000 * (-s)), 0, width);
        int y1 = std::clamp(int(y0 + 1000 * (-c)), 0, height);
        Line l{ x0 ,y0 ,x1 ,y1 };
        DrawLine(in, l, color);
    }

}
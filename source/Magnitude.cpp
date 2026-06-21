#include "Magnitude.hpp"

template <qlm::pixel_t T>
qlm::Image<qlm::ImageFormat::GRAY, std::make_unsigned_t<T>> qlm::Magnitude(const qlm::Image<qlm::ImageFormat::GRAY, T> &g_x, 
                                                                           const qlm::Image<qlm::ImageFormat::GRAY, T> &g_y, 
                                                                           const bool L2gradient)
{
    qlm::Image<qlm::ImageFormat::GRAY, std::make_unsigned_t<T>> out(g_x.width, g_x.height);

    for (int y = 0; y < g_x.height; y++)
    {
        for (int x = 0; x < g_x.width; x++)
        {
            const double pix_x = static_cast<double>(g_x.GetPixel(x, y).v);
            const double pix_y = static_cast<double>(g_y.GetPixel(x, y).v);

            std::make_unsigned_t<T> magnitude;
            if (L2gradient)
            {
                magnitude = static_cast<std::make_unsigned_t<T>>(std::sqrt(pix_x * pix_x + pix_y * pix_y));
            }
            else
            {
                magnitude = static_cast<std::make_unsigned_t<T>>(std::abs(pix_x) + std::abs(pix_y));
            }

            out.SetPixel(x, y, magnitude);
        }
    }

    return out;
}

template qlm::Image<qlm::ImageFormat::GRAY, uint16_t> qlm::Magnitude<int16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &, 
                                                                              const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &, 
                                                                              const bool);
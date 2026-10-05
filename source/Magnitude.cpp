#include "Magnitude.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>

namespace qlm
{
    template <pixel_t T, pixel_t TO>
    void Magnitude(const Image<ImageFormat::GRAY, T> &g_x,
                const Image<ImageFormat::GRAY, T> &g_y,
                Image<ImageFormat::GRAY, TO> &out,
                const bool L2gradient,
                const Rectangle<int> &roi)
    {
        // validate that the input images have the same dimensions
        assert(g_x.Width() == g_y.Width() && g_x.Height() == g_y.Height());
        assert(g_x.Width() == out.Width() && g_x.Height() == out.Height());

        auto [start_x, start_y, end_x, end_y] = ROI(roi, g_x.Width(), g_x.Height());

        for (int y = start_y; y < end_y; y++)
        {
            for (int x = start_x; x < end_x; x++)
            {
                const double pix_x = static_cast<double>(g_x.GetPixel(x, y).v);
                const double pix_y = static_cast<double>(g_y.GetPixel(x, y).v);

                TO magnitude;
                if (L2gradient)
                {
                    magnitude = static_cast<TO>(std::sqrt(pix_x * pix_x + pix_y * pix_y));
                }
                else
                {
                    magnitude = static_cast<TO>(std::clamp(std::abs(pix_x) + std::abs(pix_y),
                                                        static_cast<double>(std::numeric_limits<TO>::min()),
                                                        static_cast<double>(std::numeric_limits<TO>::max())
                                                        ));
                }

                out.SetPixel(x, y, magnitude);
            }
        }
    }

    template <pixel_t T, pixel_t TO>
    Image<ImageFormat::GRAY, TO> Magnitude(const Image<ImageFormat::GRAY, T> &g_x,
                                        const Image<ImageFormat::GRAY, T> &g_y,
                                        const bool L2gradient,
                                        const Rectangle<int> &roi)
    {
        Image<ImageFormat::GRAY, TO> out(g_x.Width(), g_x.Height());
        Magnitude<T, TO>(g_x, g_y, out, L2gradient, roi);

        return out;
    }

// Explicit instantiation for GRAY, int16_t, uint16_t 
template void qlm::Magnitude<int16_t, uint16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                qlm::Image<qlm::ImageFormat::GRAY, uint16_t> &,
                                                const bool,
                                                const Rectangle<int> &);

template qlm::Image<qlm::ImageFormat::GRAY, uint16_t> qlm::Magnitude<int16_t, uint16_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                                                       const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                                                       const bool,
                                                                                       const Rectangle<int> &);
                
// Explicit instantiation for GRAY, int16_t, int32_t
template void qlm::Magnitude<int16_t, int32_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                qlm::Image<qlm::ImageFormat::GRAY, int32_t> &,
                                                const bool,
                                                const Rectangle<int> &);
                                                
template qlm::Image<qlm::ImageFormat::GRAY, int32_t> qlm::Magnitude<int16_t, int32_t>(const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                                                       const qlm::Image<qlm::ImageFormat::GRAY, int16_t> &,
                                                                                       const bool,
                                                                                       const Rectangle<int> &);
}
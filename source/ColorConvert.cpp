#include "ColorConvert.hpp"
#include <algorithm>
#include <cmath>
#include <tuple>

namespace qlm
{
    // clamp a float to the range of T before converting (out-of-range float -> integer
    // conversion is undefined behaviour; e.g. saturated red gives Cr = 255.5)
    template<pixel_t T>
    T ClampTo(const float value)
    {
        return static_cast<T>(std::clamp(value,
                                         static_cast<float>(std::numeric_limits<T>::lowest()),
                                         static_cast<float>(std::numeric_limits<T>::max())));
    }

    template<pixel_t T>
    Pixel<ImageFormat::GRAY, T> RGB2GRAY(const Pixel<ImageFormat::RGB, T>& in)
    {
        const float fgray = 0.299f * in.r + 0.587f * in.g + 0.114f * in.b;
        return Pixel<ImageFormat::GRAY, T>(ClampTo<T>(fgray));
    }

    template<pixel_t T>
    Pixel<ImageFormat::RGB, T> GRAY2RGB(const Pixel<ImageFormat::GRAY, T>& in)
    {
        return Pixel<ImageFormat::RGB, T>(in.v, in.v, in.v);
    }

    template<pixel_t T>
    Pixel<ImageFormat::YCrCb, T> RGB2YCrCb(const Pixel<ImageFormat::RGB, T>& in)
    {
        const float delta = std::is_same_v<float, T> ? 0.5f :
                            sizeof(T) == 1 ? 128 : 32768;

        const float y = 0.299f * in.r + 0.587f * in.g + 0.114f * in.b;
        const float Cr = (in.r - y) * 0.713f + delta;
        const float Cb = (in.b - y) * 0.564f + delta;
        
        return Pixel<ImageFormat::YCrCb, T>(ClampTo<T>(y), ClampTo<T>(Cr), ClampTo<T>(Cb));
    }

    template<pixel_t T>
    Pixel<ImageFormat::RGB, T> YCrCb2RGB(const Pixel<ImageFormat::YCrCb, T>& in)
    {
        const float delta = std::is_same_v<float, T> ? 0.5f :
                            sizeof(T) == 1 ? 128 : 32768;

        float r = in.y + 1.402f * (in.cr - delta);
        float g = in.y - 0.714f * (in.cr - delta) - 0.344f * (in.cb - delta);
        float b = in.y + 1.772f * (in.cb - delta);

        return Pixel<ImageFormat::RGB, T>(ClampTo<T>(r), ClampTo<T>(g), ClampTo<T>(b));
    }

    template<pixel_t T, ImageFormat dst_frmt>
    Pixel<dst_frmt, T> RGB2HSVOrHLS(const Pixel<ImageFormat::RGB, T>& in)
    {
        // normalize R,G,B channels to range 0 to 1
        const float r = (in.r - std::numeric_limits<T>::lowest()) / (float)(std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest());
        const float g = (in.g - std::numeric_limits<T>::lowest()) / (float)(std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest());
        const float b = (in.b - std::numeric_limits<T>::lowest()) / (float)(std::numeric_limits<T>::max() - std::numeric_limits<T>::lowest());

        const float c_max = std::max({r, g, b});
        const float c_min = std::min({r, g, b});
        const float delta = c_max - c_min;

        float h = 0.0f, sat = 0.0f, val = 0.0f; // hue, saturation, value|lightness

        // h component
        // delta == 0 (pure gray): hue is undefined -> use 0 to avoid division by zero
        if (delta != 0.0f)
        {
            h = c_max == r ? (60.0f * (g - b) / delta) :
                c_max == g ? (120.0f + 60.0f * (b - r) / delta) :
                            (240.0f + 60.0f * (r - g) / delta);

            if (h < 0)
                h += 360.0f;
        }

        if constexpr (dst_frmt == ImageFormat::HSV)
        {
            // V = c_max, S = delta / c_max
            val = c_max;
            sat = c_max ? (delta / c_max) : 0.0f;
        }
        else
        {
            // L = (c_max + c_min) / 2, S = delta / (1 - |2L - 1|)
            val = (c_max + c_min) / 2.0f;
            const float denom = val < 0.5f ? (c_max + c_min) : (2.0f - (c_max + c_min));
            sat = denom != 0.0f ? (delta / denom) : 0.0f;
        }

        // only U8 or float are supported for HSV/HLS 
        if constexpr (std::is_same_v<uint8_t, T>)
        {
            val *= 255.0f;
            sat *= 255.0f;
            h /= 2.0f; // to fit in 255
        }

        if constexpr (dst_frmt == ImageFormat::HSV)
        {
            // Pixel<HSV>(hue, saturation, value)
            return Pixel<dst_frmt, T>(ClampTo<T>(h), ClampTo<T>(sat), ClampTo<T>(val));
        }
        else
        {
            // Pixel<HLS>(hue, lightness, saturation)
            return Pixel<dst_frmt, T>(ClampTo<T>(h), ClampTo<T>(val), ClampTo<T>(sat));
        }
    }

    std::tuple<float, float, float> RGBFromH(const float h, const float chroma, const float m)
    {
        // wrap the hue into [0, 360) (e.g. 8-bit HSV/HLS inputs may store h up to 255,
        // which doubles to 510 on the way in)
        const float hue = std::fmod(std::fmod(h, 360.0f) + 360.0f, 360.0f);

        const float x = chroma * (1 - std::abs(std::fmod(hue / 60.0f, 2) - 1));

        float rp{0}, gp{0}, bp{0};

        if (0.0f <= hue && hue < 60.0f)
        {
            rp = chroma;
            gp = x;
            bp = 0;
        }
        else if (60.0f <= hue && hue < 120.0f)
        {
            rp = x;
            gp = chroma;
            bp = 0;
        }
        else if (120.0f <= hue && hue < 180.0f)
        {
            rp = 0;
            gp = chroma;
            bp = x;
        }
        else if (180.0f <= hue && hue < 240.0f)
        {
            rp = 0;
            gp = x;
            bp = chroma;
        }
        else if (240.0f <= hue && hue < 300.0f)
        {
            rp = x;
            gp = 0;
            bp = chroma;
        }
        else if (300.0f <= hue && hue < 360.0f)
        {
            rp = chroma;
            gp = 0;
            bp = x;
        }

        return {rp + m, gp + m, bp + m};
    }
    
    template<pixel_t T>
    Pixel<ImageFormat::RGB, T> HLS2RGB(const Pixel<ImageFormat::HLS, T>& in)
    {
        float h = in.h, s = in.s, l = in.l;

        if constexpr (std::is_same_v<uint8_t, T>)
        {
            l /= 255.0f;
            s /= 255.0f;
            h *= 2.0f;
        }

        const float chroma = (1 - std::abs(2 * l - 1)) * s;
        const float m = l - 0.5f * chroma;

        const auto [r, g, b] = RGBFromH(h, chroma, m);

        if constexpr (std::is_same_v<uint8_t, T>)
        {
            return Pixel<ImageFormat::RGB, T>(ClampTo<T>(std::round(r * 255.0f)),
                                              ClampTo<T>(std::round(g * 255.0f)),
                                              ClampTo<T>(std::round(b * 255.0f)));
        }
        else
        {
            return Pixel<ImageFormat::RGB, T>(r, g, b);
        }
    }

    template<pixel_t T>
    Pixel<ImageFormat::RGB, T> HSV2RGB(const Pixel<ImageFormat::HSV, T>& in)
    {
        float h = in.h, s = in.s, v = in.v;

        if constexpr (std::is_same_v<uint8_t, T>)
        {
            v /= 255.0f;
            s /= 255.0f;
            h *= 2.0f;
        }

        const float chroma = v * s;
        const float m = v - chroma;

        const auto [r, g, b] = RGBFromH(h, chroma, m);

        if constexpr (std::is_same_v<uint8_t, T>)
        {
            return Pixel<ImageFormat::RGB, T>(ClampTo<T>(std::round(r * 255.0f)),
                                              ClampTo<T>(std::round(g * 255.0f)),
                                              ClampTo<T>(std::round(b * 255.0f)));
        }
        else
        {
            return Pixel<ImageFormat::RGB, T>(r, g, b);
        }
    }

    template<ImageFormat src_frmt, pixel_t T, ImageFormat dst_frmt>
    Image<dst_frmt, T> ColorConvert(const Image<src_frmt, T>& in)
    {
        // check if they are the same format
        if constexpr (dst_frmt == src_frmt)
        {
            return in;
        }

        const int width = in.width;
        const int height = in.height;

        // create the output image
        Image<dst_frmt, T> out (width, height);
        Pixel<dst_frmt, T> dst_pix;

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                // input pixel
                const Pixel<src_frmt, T>src_pix = in.GetPixel(x, y);

                if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::GRAY)
                {
                    dst_pix = RGB2GRAY(src_pix);
                }
                else if constexpr (src_frmt == ImageFormat::GRAY && dst_frmt == ImageFormat::RGB)
                {
                    dst_pix = GRAY2RGB(src_pix);
                }
                else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::HSV)
                {
                    dst_pix = RGB2HSVOrHLS<ImageFormat::HSV>(src_pix);
                }
                else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::HLS)
                {
                    dst_pix = RGB2HSVOrHLS<ImageFormat::HLS>(src_pix);
                }
                else if constexpr (src_frmt  == ImageFormat::HSV && dst_frmt == ImageFormat::RGB)
                {
                    dst_pix = HSV2RGB(src_pix);
                }
                else if constexpr (src_frmt  == ImageFormat::HLS && dst_frmt == ImageFormat::RGB)
                {
                    dst_pix = HLS2RGB(src_pix);
                }
                else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::YCrCb)
                {
                    dst_pix = RGB2YCrCb(src_pix);
                }
                else if constexpr (src_frmt == ImageFormat::YCrCb && dst_frmt == ImageFormat::RGB)
                {
                    dst_pix = YCrCb2RGB(src_pix);
                }

                // copy alpha component
                dst_pix.a = src_pix.a;
  
                out.SetPixel(x, y, dst_pix);
            }
        }

        return out;
    }
}

// Explicit instantiation for RGB2GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY>
    (const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for GRAY2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB>
(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&);

// Explicit instantiation for RGB2HSV , uint8_t
template qlm::Image<qlm::ImageFormat::HSV, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HSV>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for RGB2HLS , uint8_t
template qlm::Image<qlm::ImageFormat::HLS, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HLS	>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for HSV2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t, qlm::ImageFormat::RGB>
(const qlm::Image<qlm::ImageFormat::HSV, uint8_t>&);

// Explicit instantiation for HLS2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::HLS, uint8_t, qlm::ImageFormat::RGB>
(const qlm::Image<qlm::ImageFormat::HLS, uint8_t>&);

// Explicit instantiation for RGB2HSV , uint8_t
template qlm::Image<qlm::ImageFormat::YCrCb, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::YCrCb>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for YCrCb2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::YCrCb, uint8_t, qlm::ImageFormat::RGB>
(const qlm::Image<qlm::ImageFormat::YCrCb, uint8_t>&);

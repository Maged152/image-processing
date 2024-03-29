#include "ColorConvert.h"
#include <algorithm>
#include <cmath>

namespace qlm
{
    template<ImageFormat src_frmt, pixel_t src_t, ImageFormat dst_frmt, pixel_t dst_t>
    Image<dst_frmt, dst_t> ColorConvert(const Image<src_frmt, src_t>& in)
    {
        unsigned int width = in.Width();
        unsigned int height = in.Height();
        // create the output image
        Image<dst_frmt, dst_t> out;
        out.create(width, height);
        // check if they are the same format & same data type
        if constexpr (dst_frmt == src_frmt && std::is_same_v<src_t, dst_t>)
        {
            // copy
            out = in;
            
            return out;
        }

        Pixel<dst_frmt, dst_t> dst_pix;
        Pixel<src_frmt, src_t> src_pix;

        dst_t dst_min = std::numeric_limits<dst_t>::lowest();
        dst_t dst_max = std::numeric_limits<dst_t>::max();

        src_t src_min = std::numeric_limits<src_t>::lowest();
        src_t src_max = std::numeric_limits<src_t>::max();

        for (int i = 0; i < width * height; i++)
        {
            // input pixel
            src_pix = in.GetPixel(i);
            if constexpr (dst_frmt == src_frmt)
            {
                // cast
                dst_pix = src_pix;	
            }
            else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::GRAY)
            {
                // RGB to GRAY
                float fgray = 0.299f * src_pix.r + 0.587f * src_pix.g + 0.114f * src_pix.b;
                // normalize the gray value
                fgray = (fgray - src_min) / (float)(src_max - src_min);
                // denormalize
                dst_pix.v = static_cast<dst_t>(fgray * (dst_max - dst_min) + dst_min);
            }
            else if constexpr (src_frmt == ImageFormat::GRAY && dst_frmt == ImageFormat::RGB)
            {
                // GRAY to RGB
                dst_pix.r = dst_pix.g = dst_pix.b = static_cast<dst_t>(std::clamp<cast_t<src_t, dst_t>>(src_pix.v, dst_min, dst_max));
            }
            else if constexpr (src_frmt == ImageFormat::RGB && (dst_frmt == ImageFormat::HSV || dst_frmt == ImageFormat::HLS))
            {
                // normalize R,G,B channels to range 0 to 1
                float r = (src_pix.r - src_min) / (float)(src_max - src_min);
                float g = (src_pix.g - src_min) / (float)(src_max - src_min);
                float b = (src_pix.b - src_min) / (float)(src_max - src_min);

                float c_max = std::max({r, g, b});
                float c_min = std::min({r, g, b});
                float delta = c_max - c_min;

                // h component
                float h = c_max == r ? (60.0f * (g - b) / delta) :
                    c_max == g ? (120.0f + 60.0f * (b - r) / delta) :
                    c_max == b ? (240.0f + 60.0f * (r - g) / delta) : 0;

                if (h < 0)
                    h += 360.0f;

                if constexpr (dst_frmt == ImageFormat::HSV)
                {
                    // RGB to HSV
                    // v componet
                    float v = c_max;
                    // s componet
                    float s = c_max ? (delta / c_max) : 0;
                    
                    /*
                    * the output
                    * 0 <= v <= 1
                    * 0 <= s <= 1
                    * 0 <= h <= 360
                    */
                    if constexpr (std::is_same_v<float, dst_t>)
                    {
                        // v componet
                        dst_pix.v = v;
                        // s componet
                        dst_pix.s = s;
                        // h component
                        dst_pix.h = h;
                    }
                    else
                    {
                        // scale up
                        // v componet
                        dst_pix.v = static_cast<dst_t>(v * (dst_max - dst_min) + dst_min);
                        // s componet
                        dst_pix.s = static_cast<dst_t>(s * (dst_max - dst_min) + dst_min);
                        // h component
                        if constexpr (std::is_same_v<uint8_t, dst_t>)
                        {
                            // h = h /2 "to fit in 255"
                            dst_pix.h = static_cast<dst_t>(h / 2);
                        }
                        else
                        {
                            dst_pix.h = static_cast<dst_t>(h);
                        }
                        
                    }

                }
                else
                {
                    // RGB to HLS
                    // l componet
                    float l = (c_max + c_min) / 2.0f;
                    // s componet
                    float s = l < 0.5f ? delta / (c_max + c_min) : delta / (2.0f - c_max + c_min);
                    
                    /*
                    * the output
                    * 0 <= l <= 1
                    * 0 <= s <= 1
                    * 0 <= h <= 360
                    */
                    if constexpr (std::is_same_v<float, dst_t>)
                    {
                        // v componet
                        dst_pix.l = l;
                        // s componet
                        dst_pix.s = s;
                        // h component
                        dst_pix.h = h;
                    }
                    else
                    {
                        // scale up
                        // v componet
                        dst_pix.l = static_cast<dst_t>(l * (dst_max - dst_min) + dst_min);
                        // s componet
                        dst_pix.s = static_cast<dst_t>(s * (dst_max - dst_min) + dst_min);
                        // h component
                        if constexpr (std::is_same_v<uint8_t, dst_t>)
                        {
                            // h = h /2 "to fit in 255"
                            dst_pix.h = static_cast<dst_t>(h / 2);
                        }
                        else
                        {
                            dst_pix.h = static_cast<dst_t>(h);
                        }

                    }

                }
            }
            else if constexpr ((src_frmt  == ImageFormat::HSV || src_frmt == ImageFormat::HLS) && dst_frmt == ImageFormat::RGB)
            {
                float rp{0}, gp{0}, bp{0}; // intermediate red, green, blue values
                // normalize s channel to range 0 to 1
                float s = (src_pix.s - src_min) / (float)(src_max - src_min);
                // h component
                wider_t<src_t> h = src_pix.h;
                if (std::is_same_v<src_t, uint8_t>)
                    h *= 2;

                float hp = h / 60.0f; // hue prime
                float m{ 0 }, c{ 0 }, x{ 0 };
            
                if constexpr (src_frmt == ImageFormat::HSV)
                {
                    // HSV to RGB
                    // normalize v channel to range 0 to 1
                    float v = (src_pix.v - src_min) / (float)(src_max - src_min);
                    c = v * s; // chroma
                    m = v - c; // value minus chroma
                }
                else
                {
                    // HLS to RGB
                    // normalize l channel to range 0 to 1
                    float l = (src_pix.l - src_min) / (float)(src_max - src_min);
                    c = (1 - std::abs(2 * l - 1)) * s; // chroma
                    m = l - 0.5f * c;
                }

                x = c * (1 - std::abs(std::fmod(hp, 2) - 1)); // intermediate value

                if (0.0f <= h && h < 60.0f)
                {
                    rp = c;
                    gp = x;
                    bp = 0;
                }
                else if (60.0f <= h && h < 120.0f)
                {
                    rp = x;
                    gp = c;
                    bp = 0;
                }
                else if (120.0f <= h && h < 180.0f)
                {
                    rp = 0;
                    gp = c;
                    bp = x;
                }
                else if (180.0f <= h && h < 240.0f)
                {
                    rp = 0;
                    gp = x;
                    bp = c;
                }
                else if (240.0f <= h && h < 300.0f)
                {
                    rp = x;
                    gp = 0;
                    bp = c;
                }
                else if (300.0f <= h && h < 360.0f)
                {
                    rp = c;
                    gp = 0;
                    bp = x;
                }
                else
                {
                    rp = 0;
                    gp = 0;
                    bp = 0;
                }

                if (std::is_same_v<dst_t, float>)
                {
                    dst_pix.r = rp + m;
                    dst_pix.g = gp + m;
                    dst_pix.b = bp + m;
                }
                else
                {
                    // scale up
                    dst_pix.r = static_cast<dst_t>((rp + m) * (dst_max - dst_min) + dst_min);
                    dst_pix.g = static_cast<dst_t>((gp + m) * (dst_max - dst_min) + dst_min);
                    dst_pix.b = static_cast<dst_t>((bp + m) * (dst_max - dst_min) + dst_min);
                }

            }
            else if constexpr (src_frmt == ImageFormat::RGB && dst_frmt == ImageFormat::YCrCb)
            {
                float delta = std::is_same_v<float, src_t> ? 0.5f : (src_max / 2 + 1);
                // Y, Cr, Cb component
                float y = 0.299f * src_pix.r + 0.587f * src_pix.g + 0.114f * src_pix.b;
                float Cr = (src_pix.r - y) * 0.713f + delta;
                float Cb = (src_pix.b - y) * 0.564f + delta;
                if constexpr (std::is_same_v<float, src_t>)
                {
                    dst_pix.y  = y ;
                    dst_pix.cr = Cr;
                    dst_pix.cb = Cb;
                }
                else
                {
                    // normalization
                    y = (y - src_min) / (float)(src_max - src_min);
                    Cr = (Cr - src_min) / (float)(src_max - src_min);
                    Cb = (Cb - src_min) / (float)(src_max - src_min);
                    // denormalize
                    dst_pix.y = static_cast<dst_t>(y * (dst_max - dst_min) + dst_min);
                    dst_pix.cr = static_cast<dst_t>(Cr * (dst_max - dst_min) + dst_min);
                    dst_pix.cb = static_cast<dst_t>(Cb * (dst_max - dst_min) + dst_min);
                }
            }
            else if constexpr (src_frmt == ImageFormat::YCrCb && dst_frmt == ImageFormat::RGB)
            {
                float delta = std::is_same_v<float, src_t> ? 0.5f : (src_max / 2 + 1);
                // R, G, B component
                float r = src_pix.y + 1.403f * (src_pix.cr - delta);
                float g = src_pix.y - 0.714f * (src_pix.cr - delta) - 0.344f * (src_pix.cb - delta);
                float b = src_pix.y + 1.773f * (src_pix.cb - delta);
                if constexpr (std::is_same_v<float, src_t>)
                {
                    dst_pix.r = r;
                    dst_pix.g = g;
                    dst_pix.b = b;
                }
                else
                {
                    // normalization
                    r = (r - src_min) / (float)(src_max - src_min);
                    g = (g - src_min) / (float)(src_max - src_min);
                    b = (b - src_min) / (float)(src_max - src_min);
                    // clamp
                    r = std::clamp(r, 0.0f, 1.0f);
                    g = std::clamp(g, 0.0f, 1.0f);
                    b = std::clamp(b, 0.0f, 1.0f);
                    // denormalize
                    dst_pix.r = static_cast<dst_t>(r * (dst_max - dst_min) + dst_min);
                    dst_pix.g = static_cast<dst_t>(g * (dst_max - dst_min) + dst_min);
                    dst_pix.b = static_cast<dst_t>(b * (dst_max - dst_min) + dst_min);
                }
            }
            // copy alpha component
            dst_pix.a = static_cast<dst_t>(std::clamp<cast_t<src_t, dst_t>>(src_pix.a, dst_min, dst_max));
            // set output pixel
            out.SetPixel(i, dst_pix);
        }

        return out;
    }
}

// Explicit instantiation for RGB2GRAY , uint8_t
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>
    (const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for GRAY2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&);

// Explicit instantiation for RGB2HSV , uint8_t
template qlm::Image<qlm::ImageFormat::HSV, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HSV, uint8_t>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for RGB2HLS , uint8_t
template qlm::Image<qlm::ImageFormat::HLS, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::HLS	, uint8_t>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for HSV2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::HSV, uint8_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::HSV, uint8_t>&);

// Explicit instantiation for HLS2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::HLS, uint8_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::HLS, uint8_t>&);

// Explicit instantiation for RGB2HSV , uint8_t
template qlm::Image<qlm::ImageFormat::YCrCb, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::YCrCb, uint8_t>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

// Explicit instantiation for YCrCb2RGB , uint8_t
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::YCrCb, uint8_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::YCrCb, uint8_t>&);

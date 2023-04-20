#include "ColorConvert.h"
#include <algorithm>


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

		dst_t min_val = std::numeric_limits<dst_t>::lowest();
		dst_t max_val = std::numeric_limits<dst_t>::max();

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
				// cast
				dst_pix.v = static_cast<dst_t>(std::clamp<float>(fgray, min_val, max_val));
				dst_pix.a = static_cast<dst_t>(std::clamp<cast_t<src_t, dst_t>>(src_pix.a, min_val, max_val));
			}
			else if constexpr (src_frmt == ImageFormat::GRAY && dst_frmt == ImageFormat::RGB)
			{
				// GRAY to RGB
				dst_pix.r = dst_pix.g = dst_pix.b = static_cast<dst_t>(std::clamp<cast_t<src_t, dst_t>>(src_pix.v, min_val, max_val));
				dst_pix.a = static_cast<dst_t>(std::clamp<cast_t<src_t, dst_t>>(src_pix.a, min_val, max_val));
			}
			else if constexpr (src_frmt == ImageFormat::RGB && (dst_frmt == ImageFormat::HSV || dst_frmt == ImageFormat::HLS))
			{
				// normalize R,G,B channels to range 0 to 1
				float r = (src_pix.r - min_val) / (float)(max_val - min_val);
				float g = (src_pix.g - min_val) / (float)(max_val - min_val);
				float b = (src_pix.b - min_val) / (float)(max_val - min_val);

				float c_max = std::max(r, g, b);
				float c_min = std::min(r, g, b);
				float delta = c_max - c_min;

				if constexpr (dst_frmt == ImageFormat::HSV)
				{
					// RGB to HSV
					if constexpr (std::is_same_v<float, dst_t>)
					{
						// v componet
						dst_pix.v = c_max;
						// s componet
						dst_pix.s = c_max ? (delta / c_max) : 0;
						// h component
						dst_pix.h = c_max == r ? (60.0f * (g - b) / delta) :
									c_max == g ? (120.0f + 60.0f * (b - r) / delta) :
									c_max == b ? (240.0f + 60.0f * (r - g) / delta) : 0;
					}
					else
					{
						// scale up
					}

				}
				else
				{
					// RGB to HLS

				}
			}
			// set output pixel
			out.SetPixel(i, dst_pix);
		}

		return out;
	}
}

// Explicit instantiation for RGB2GRAY
template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, uint8_t>
	(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, uint8_t, qlm::ImageFormat::GRAY, int16_t>
(const qlm::Image<qlm::ImageFormat::RGB, uint8_t>&);

template qlm::Image<qlm::ImageFormat::GRAY, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, int16_t, qlm::ImageFormat::GRAY, uint8_t>
(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&);

template qlm::Image<qlm::ImageFormat::GRAY, int16_t>
qlm::ColorConvert<qlm::ImageFormat::RGB, int16_t, qlm::ImageFormat::GRAY, int16_t>
(const qlm::Image<qlm::ImageFormat::RGB, int16_t>&);

// Explicit instantiation for GRAY2RGB
template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&);

template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, uint8_t, qlm::ImageFormat::RGB, int16_t>
(const qlm::Image<qlm::ImageFormat::GRAY, uint8_t>&);

template qlm::Image<qlm::ImageFormat::RGB, uint8_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, int16_t, qlm::ImageFormat::RGB, uint8_t>
(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&);

template qlm::Image<qlm::ImageFormat::RGB, int16_t>
qlm::ColorConvert<qlm::ImageFormat::GRAY, int16_t, qlm::ImageFormat::RGB, int16_t>
(const qlm::Image<qlm::ImageFormat::GRAY, int16_t>&);
